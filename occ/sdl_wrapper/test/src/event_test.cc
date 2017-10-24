#include <gtest/gtest.h>

#include "sdl_stub.h"
#include "event.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::DoAll;

class EventTest : public ::testing::Test
{
 protected:
  void SetUp() override { SDLStub::get(); }
  void TearDown() override { SDLStub::reset(); }
};

TEST_F(EventTest, poll_event)
{
  auto event = Event::create();
  ASSERT_TRUE(event.get() != nullptr);

  SDL_Event event1;
  SDL_Event event2;
  SDL_Event event3;
  SDL_Event event4;
  event1.key.type = SDL_KEYDOWN;
  event1.key.keysym.sym = SDLK_UP;
  event2.key.type = SDL_KEYDOWN;
  event2.key.keysym.sym = SDLK_RIGHT;
  event3.key.type = SDL_KEYDOWN;
  event3.key.keysym.sym = SDLK_z;
  event4.key.type = SDL_KEYDOWN;
  event4.key.keysym.sym = SDLK_RETURN;
  EXPECT_CALL(SDLStub::get(), SDL_PollEvent(_)).WillOnce(DoAll(SetArgPointee<0>(event1),
                                                               Return(1)))
                                               .WillOnce(DoAll(SetArgPointee<0>(event2),
                                                               Return(1)))
                                               .WillOnce(DoAll(SetArgPointee<0>(event3),
                                                               Return(1)))
                                               .WillOnce(DoAll(SetArgPointee<0>(event4),
                                                               Return(1)))
                                               .WillOnce(Return(0));

  Input input;
  event->poll_event(&input);

  EXPECT_TRUE(input.up.pressed);
  EXPECT_TRUE(input.right.pressed);
  EXPECT_TRUE(input.z.pressed);
  EXPECT_TRUE(input.enter.pressed);

  EXPECT_FALSE(input.up.repeated);
  EXPECT_FALSE(input.right.repeated);
  EXPECT_FALSE(input.z.repeated);
  EXPECT_FALSE(input.enter.repeated);

  EXPECT_FALSE(input.down.pressed);
  EXPECT_FALSE(input.down.repeated);
  EXPECT_FALSE(input.left.pressed);
  EXPECT_FALSE(input.left.repeated);
  EXPECT_FALSE(input.x.pressed);
  EXPECT_FALSE(input.x.repeated);
  EXPECT_FALSE(input.num_1.pressed);
  EXPECT_FALSE(input.num_1.repeated);
  EXPECT_FALSE(input.num_2.pressed);
  EXPECT_FALSE(input.num_2.repeated);
  EXPECT_FALSE(input.space.pressed);
  EXPECT_FALSE(input.space.repeated);

  EXPECT_FALSE(input.quit);
}

TEST_F(EventTest, poll_event_repeated)
{
  auto event = Event::create();
  ASSERT_TRUE(event.get() != nullptr);

  SDL_Event event1;
  SDL_Event event2;
  event1.key.type = SDL_KEYDOWN;
  event1.key.keysym.sym = SDLK_UP;
  event2.key.type = SDL_KEYDOWN;
  event2.key.keysym.sym = SDLK_RIGHT;
  EXPECT_CALL(SDLStub::get(), SDL_PollEvent(_)).WillOnce(DoAll(SetArgPointee<0>(event1),
                                                               Return(1)))
                                               .WillOnce(DoAll(SetArgPointee<0>(event2),
                                                               Return(1)))
                                               .WillOnce(Return(0));

  Input input;
  event->poll_event(&input);

  EXPECT_TRUE(input.up.pressed);
  EXPECT_TRUE(input.right.pressed);

  EXPECT_FALSE(input.up.repeated);
  EXPECT_FALSE(input.right.repeated);


  event1.key.type = SDL_KEYDOWN;
  event1.key.keysym.sym = SDLK_UP;
  event2.key.type = SDL_KEYUP;
  event2.key.keysym.sym = SDLK_RIGHT;
  EXPECT_CALL(SDLStub::get(), SDL_PollEvent(_)).WillOnce(DoAll(SetArgPointee<0>(event1),
                                                               Return(1)))
                                               .WillOnce(DoAll(SetArgPointee<0>(event2),
                                                               Return(1)))
                                               .WillOnce(Return(0));

  event->poll_event(&input);

  EXPECT_TRUE(input.up.pressed);
  EXPECT_FALSE(input.right.pressed);

  EXPECT_TRUE(input.up.repeated);
  EXPECT_FALSE(input.right.repeated);
}

TEST_F(EventTest, poll_event_early_quit)
{
  auto event = Event::create();
  ASSERT_TRUE(event.get() != nullptr);

  SDL_Event event1;
  SDL_Event event2;
  event1.type = SDL_KEYDOWN;
  event1.key.keysym.sym = SDLK_UP;
  event2.type = SDL_QUIT;
  EXPECT_CALL(SDLStub::get(), SDL_PollEvent(_)).WillOnce(DoAll(SetArgPointee<0>(event1),
                                                               Return(1)))
                                               .WillOnce(DoAll(SetArgPointee<0>(event2),
                                                               Return(1)));

  Input input;
  event->poll_event(&input);
  EXPECT_TRUE(input.up.pressed);
  EXPECT_FALSE(input.up.repeated);
  EXPECT_TRUE(input.quit);


  event1.type = SDL_KEYUP;
  event1.key.keysym.sym = SDLK_UP;
  event2.type = SDL_KEYDOWN;
  event2.key.keysym.sym = SDLK_ESCAPE;
  EXPECT_CALL(SDLStub::get(), SDL_PollEvent(_)).WillOnce(DoAll(SetArgPointee<0>(event1),
                                                               Return(1)))
                                               .WillOnce(DoAll(SetArgPointee<0>(event2),
                                                               Return(1)));

  event->poll_event(&input);
  EXPECT_FALSE(input.up.pressed);
  EXPECT_FALSE(input.up.repeated);
  EXPECT_TRUE(input.quit);
}
