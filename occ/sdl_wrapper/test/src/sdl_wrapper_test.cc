#include <gtest/gtest.h>

#include "sdl_stub.h"
#include "sdl_wrapper.h"

using ::testing::_;
using ::testing::Return;

class SDLWrapperTest : public ::testing::Test
{
 protected:
  void SetUp() override { SDLStub::get(); }
  void TearDown() override { SDLStub::reset(); }
};

TEST_F(SDLWrapperTest, init_ok)
{
  auto wrapper = SDLWrapper::create();
  ASSERT_TRUE(wrapper.get() != nullptr);

  EXPECT_CALL(SDLStub::get(), SDL_Init(_));
  EXPECT_CALL(SDLStub::get(), TTF_Init());
  EXPECT_TRUE(wrapper->init());
}

TEST_F(SDLWrapperTest, init_nok)
{
  auto wrapper = SDLWrapper::create();
  ASSERT_TRUE(wrapper.get() != nullptr);

  EXPECT_CALL(SDLStub::get(), SDL_Init(_)).WillOnce(Return(-1));
  EXPECT_CALL(SDLStub::get(), SDL_GetError());
  EXPECT_FALSE(wrapper->init());
}

TEST_F(SDLWrapperTest, get_tick)
{
  auto wrapper = SDLWrapper::create();
  ASSERT_TRUE(wrapper.get() != nullptr);

  const auto tick = 123u;
  EXPECT_CALL(SDLStub::get(), SDL_GetTicks()).WillOnce(Return(tick));
  EXPECT_EQ(tick, wrapper->get_tick());
}

