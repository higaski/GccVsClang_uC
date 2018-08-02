#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "sys.hpp"

#define BLINKY 0
#define HELLO_WORLD 1
#define I2C_SLAVE 2
#define ENCODER 3
#define TYPE_TREE_MENU 4
#define LUA 5

#define BUILD BLINKY

#if (BUILD == BLINKY)

int main() {
  sysInit();

  for (;;) {
    HAL_Delay(1000);
    HAL_GPIO_TogglePin(GPIOD,
                       GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12);
  }
}

#elif (BUILD == HELLO_WORLD)

#  include <trace.h>

int main() {
  sysInit();

  for (;;) {
    HAL_Delay(1000);
    trace_printf("Hello World\n");
  }
}

#elif (BUILD == I2C_SLAVE)

#  include <array>
#  include <trace.h>

int main() {
  sysInit();

  constexpr auto size = 42;
  std::array<uint8_t, size> buf{};

  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  __HAL_RCC_I2C1_CLK_ENABLE();
  I2C_HandleTypeDef hi2c1;
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 84;  // (42 ;) )
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c1);

  for (auto i = 0; i < size; i++)
    buf[i] = i;

  for (;;) {
    while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) == GPIO_PIN_RESET) {
    }

    auto status =
        HAL_I2C_Slave_Transmit(&hi2c1, (uint8_t*)(&buf[0]), size, 10000);
    switch (status) {
      case HAL_OK:
        trace_printf("HAL_OK\n");
        break;

      case HAL_BUSY:
        trace_printf("HAL_BUSY\n");
        break;

      case HAL_TIMEOUT:
        trace_printf("HAL_TIMEOUT\n");
        break;

      case HAL_ERROR:
        trace_printf("HAL_ERROR\n");
        break;
    }
  }
}

#elif (BUILD == ENCODER)

#  include <trace.h>

int main() {
  sysInit();

  TIM_HandleTypeDef htim3;

  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_TIM3_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  TIM_Encoder_InitTypeDef sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK) {
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK) {
  }

  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);

  for (;;) {
    trace_printf("%d\n", TIM3->CNT);
    HAL_Delay(1000);
  }
}

#elif (BUILD == TYPE_TREE_MENU)

#  include <vector>
#  include <stdarg.h>
#  include "util.hpp"
#  include "tree.hpp"

#  define LEFT '4'
#  define RIGHT '6'
#  define PREVIOUS '8'
#  define EXIT '0'

UART_HandleTypeDef huart3;

void usart_printf(const char* format, ...) {
  int ret;
  va_list ap;

  va_start(ap, format);
  static char buf[128];

  ret = vsnprintf(&buf[0], sizeof(buf), format, ap);
  if (ret > 0) {
    HAL_UART_Transmit(&huart3,
                      reinterpret_cast<uint8_t*>(&buf[0]),
                      static_cast<size_t>(ret),
                      1000);
  }

  va_end(ap);
}

using value_type = uint8_t;

void print_choice(char choice) {
  switch (choice) {
    case LEFT:
      usart_printf("left ");
      break;

    case RIGHT:
      usart_printf("right ");
      break;

    case PREVIOUS:
      usart_printf("previous ");
      break;
  }
}

void print_node(char choice, const char* str) {
  print_choice(choice);
  usart_printf("leads to node %s\n", str);
}

void print_dead_end(char choice) {
  print_choice(choice);
  usart_printf("leads to a dead end\n");
}

void vector_left(std::vector<value_type>& v) {
  v.back() = 1;
  v.push_back(0);
  v.push_back(0);
}

void vector_right(std::vector<value_type>& v) {
  v.back() = 1;
  v.push_back(1);
  v.push_back(0);
}

bool vector_previos(std::vector<value_type>& v) {
  if (v.size() >= 3) {
    v.pop_back();
    v.pop_back();
    v.back() = 0;
    return true;
  } else
    return false;
}

void go(char choice, std::vector<value_type>& v) {
  switch (choice) {
    case LEFT:
      vector_left(v);
      break;

    case RIGHT:
      vector_right(v);
      break;

    case PREVIOUS:
      if (!vector_previos(v)) {
        print_dead_end(choice);
        return;
      }
      break;
  }

  auto str{visit(
      [](auto v) -> const char* {
        if constexpr (std::is_same_v<decltype(v), Node>)
          return v.str;
        else
          return nullptr;
      },
      tree,
      gsl::make_span(&v[0], v.size()))};

  if (str)
    print_node(choice, *str);
  else {
    print_dead_end(choice);
    vector_previos(v);
  }
}

int getchar() {
  char c;
  HAL_StatusTypeDef status{HAL_ERROR};
  do {
    status = HAL_UART_Receive(&huart3, reinterpret_cast<uint8_t*>(&c), 1, 1000);
  } while (status != HAL_OK);

  return c;
}

int main() {
  sysInit();

  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_USART3_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart3);

  usart_printf("       +---+\n");
  usart_printf("       |   |\n");
  usart_printf("       | a |\n");
  usart_printf("       |   |\n");
  usart_printf("  +----+---+----+\n");
  usart_printf("+-v-+         +-v-+\n");
  usart_printf("|   |         |   |\n");
  usart_printf("| b |         | c |\n");
  usart_printf("|   |         |   |\n");
  usart_printf("+---+      +--+---+--+\n");
  usart_printf("           |         |\n");
  usart_printf("         +-v-+     +-v-+\n");
  usart_printf("         |   |     |   |\n");
  usart_printf("         | d |     | e |\n");
  usart_printf("         |   |     |   |\n");
  usart_printf("         +---+--+  +---+\n");
  usart_printf("                |\n");
  usart_printf("              +-v-+\n");
  usart_printf("              |   |\n");
  usart_printf("              | f |\n");
  usart_printf("              |   |\n");
  usart_printf("              +---+\n");

  auto v{std::vector<value_type>{}};
  v.reserve(8);
  v.push_back(0);
  usart_printf("you start at node a\n");
  usart_printf("\n");

  for (;;) {
    char c = getchar();

    switch (c) {
      case LEFT:
        go(LEFT, v);
        break;

      case RIGHT:
        go(RIGHT, v);
        break;

      case PREVIOUS:
        go(PREVIOUS, v);
        break;

      case EXIT:
        usart_printf("bye\n");
        return 0;
        break;

      default:
        usart_printf("wrong input\n");
        break;
    }

    usart_printf("\n");
  }
}

#elif (BUILD == LUA)

#  include <lua.hpp>
#  include <trace.h>

static lua_State* create_lua_counter(char* err, size_t errlen) {
  lua_State* L;

  L = luaL_newstate();
  if (L == NULL)
    return NULL;

  luaL_openlibs(L);

  if (luaL_dofile(L, "counter.lua")) {
    snprintf(err, errlen, "Could not load counter module");
    lua_close(L);
    return NULL;
  }

  lua_getfield(L, -1, "new");

  if (!lua_isfunction(L, -1)) {
    snprintf(err, errlen, "New not a valid function");
    lua_close(L);
    return NULL;
  }

  lua_insert(L, -2);

  lua_pushinteger(L, 0);

  if (lua_pcall(L, 2, 2, 0) != 0) {
    snprintf(err, errlen, "%s", lua_tostring(L, -1));
    lua_close(L);
    return NULL;
  }

  if (lua_type(L, -2) == LUA_TNIL) {
    snprintf(err, errlen, "%s", lua_tostring(L, -1));
    lua_close(L);
    return NULL;
  }

  lua_pop(L, 1);

  if (lua_type(L, -1) != LUA_TTABLE) {
    snprintf(err, errlen, "Invalid type (%d) returned by new", lua_type(L, -1));
    lua_close(L);
    return NULL;
  }

  return L;
}

static void counter_example(lua_State* L) {
  int n;
  int isnum;

  lua_getfield(L, -1, "add");
  lua_pushvalue(L, -2);
  lua_pushnumber(L, 4);
  if (lua_pcall(L, 2, 0, 0) != 0) {
    printf("Error calling addn");
    return;
  }

  lua_getfield(L, -1, "decrement");
  lua_pushvalue(L, -2);
  if (lua_pcall(L, 1, 0, 0) != 0) {
    printf("Error calling decrementn");
    return;
  }

  lua_getfield(L, -1, "getval");
  lua_pushvalue(L, -2);
  if (lua_pcall(L, 1, 1, 0) != 0) {
    printf("Error calling getvaln");
    return;
  }
  n = lua_tointegerx(L, -1, &isnum);
  if (!isnum) {
    printf("Error getval didn't return a numbern");
    return;
  }
  printf("val=%dn", n);

  lua_remove(L, -1);

  lua_getfield(L, -1, "subtract");
  lua_pushvalue(L, -2);
  lua_pushnumber(L, -2);
  if (lua_pcall(L, 2, 0, 0) != 0) {
    printf("Error calling subtractn");
    return;
  }

  lua_getfield(L, -1, "increment");
  lua_pushvalue(L, -2);
  if (lua_pcall(L, 1, 0, 0) != 0) {
    printf("Error calling incrementn");
    return;
  }

  lua_getglobal(L, "print");
  lua_pushvalue(L, -2);
  if (lua_pcall(L, 1, 0, 0) != 0) {
    printf("Error calling print(c)n");
    return;
  }

  printf("%sn", luaL_tolstring(L, -1, NULL));

  lua_remove(L, -1);
}

int main(int argc, char** argv) {
  lua_State* L;
  char err[256];

  L = create_lua_counter(err, sizeof(err));
  if (L == NULL) {
    trace_printf("Error: %sn", err);
    return 0;
  }

  counter_example(L);

  lua_close(L);
  return 0;
}

#endif
