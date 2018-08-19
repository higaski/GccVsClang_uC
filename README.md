# GccVsClang_uC
GccVsClang_uC is a test project for the [STM32F4Discovery board](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) which compares binary sizes of usual optimization levels for embedded software (O2, Os and Clang's Oz) between GCC and Clang. 

# Test cases
GccVsClang_uC currently contains 6 different test cases I copied from other unrelated projects of mine. The macro _BUILD_ inside _main.cpp_ is used to choose the test case.

### 1. BLINKY
_BLINKY_ toggles all 4 LEDs on the board in an intervall of 1s.

### 2. HELLO_WORLD
_HELLO_WORLD_ uses [SEMIHOSTING](http://www.keil.com/support/man/docs/armcc/armcc_pge1358787046598.htm) to print a "Hello World\n" message to the debugger console.

### 3. I2C_SLAVE
_I2C_SLAVE_ utilizes the first I2C peripheral (I2C1) of the board to slave transmit some bytes. Each transmission is started by resetting Pin7 of GPIOC.

### 4. ENCODER
_ENCODER_ initializes TIM3 to run in encoder mode. Pin6 and 7 of GPIOA are used as encoder inputs. The current value of TIM3 is output to the debugger console with SEMIHOSTING.

### 5. TYPE_TREE_MENU
_TYPE_TREE_MENU_ uses a binary tree made with std::pair to create a browsable menu. The user input is continuously red from serial port USART3 and allows browsing the menu by sending the following characters
- '4' left node
- '6' right node
- '8' previous node
- '0' exit
The current position in the binary tree is sent back on the serial port.

### 6. LUA
_LUA_ embedds [Lua 5.3.4](https://www.lua.org/) and runs some Lua code...

### Caveats
Code which contains the libstdc++ version of std::variant currently won't compile with Clang 6.0.1 because of some issue with friend template functions.


# Results
Blabla -ffunction-section usw.

### BLINKY
| Compiler    | Optimizations | Text     | Data |
| ----------- | ------------- | -------- | ---- |
| GCC 7.3.0   | -O2           | 4316     | 20   |
| GCC 7.3.0   | -Os           | 4036     | 20   |
| GCC 7.3.0   | -O2 -flto     | 2520     | 12   |
| GCC 7.3.0   | -Os -flto     | 2480     | 12   |
| GCC 8.2.0   | -O2           | 4320     | 20   |
| GCC 8.2.0   | -Os           | 4044     | 20   |
| GCC 8.2.0   | -O2 -flto     | 2524     | 12   |
| GCC 8.2.0   | -Os -flto     | 2480     | 12   |
| Clang 6.0.1 | -O2           | 4208     | 20   |
| Clang 6.0.1 | -Os           | 4160     | 20   |
| Clang 6.0.1 | -Oz           | **4008** | 20   |
| Clang 6.0.1 | -O2 -flto     | 2784     | 12   |
| Clang 6.0.1 | -Os -flto     | 2504     | 12   |
| Clang 6.0.1 | -Oz -flto     | **2412** | 12   |

### HELLO_WORLD
| Compiler    | Optimizations | Text     | Data |
| ----------- | ------------- | -------- | ---- |
| GCC 7.3.0   | -O2           | 6984     | 120  |
| GCC 7.3.0   | -Os           | 6696     | 120  |
| GCC 7.3.0   | -O2 -flto     | 5196     | 112  |
| GCC 7.3.0   | -Os -flto     | 5140     | 112  |
| GCC 8.2.0   | -O2           | 6996     | 120  |
| GCC 8.2.0   | -Os           | 6712     | 120  |
| GCC 8.2.0   | -O2 -flto     | 5208     | 112  |
| GCC 8.2.0   | -Os -flto     | 5148     | 112  |
| Clang 6.0.1 | -O2           | 6892     | 120  |
| Clang 6.0.1 | -Os           | 6840     | 120  |
| Clang 6.0.1 | -Oz           | **6668** | 120  |
| Clang 6.0.1 | -O2 -flto     | 5444     | 112  |
| Clang 6.0.1 | -Os -flto     | 5164     | 112  |
| Clang 6.0.1 | -Oz -flto     | **5060** | 112  |

### I2C_SLAVE
| Compiler    | Optimizations | Text     | Data |
| ----------- | ------------- | -------- | ---- |
| GCC 7.3.0   | -O2           | 8080     | 120  |
| GCC 7.3.0   | -Os           | **7736** | 120  |
| GCC 7.3.0   | -O2 -flto     | 6552     | 112  |
| GCC 7.3.0   | -Os -flto     | **6360** | 112  |
| GCC 8.2.0   | -O2           | 8092     | 120  |
| GCC 8.2.0   | -Os           | 7760     | 120  |
| GCC 8.2.0   | -O2 -flto     | 6568     | 112  |
| GCC 8.2.0   | -Os -flto     | 6368     | 112  |
| Clang 6.0.1 | -O2           | 8232     | 120  |
| Clang 6.0.1 | -Os           | 8140     | 120  |
| Clang 6.0.1 | -Oz           | 7904     | 120  |
| Clang 6.0.1 | -O2 -flto     | 6880     | 112  |
| Clang 6.0.1 | -Os -flto     | 6404     | 112  |
| Clang 6.0.1 | -Oz -flto     | 6432     | 112  |

### ENCODER
| Compiler    | Optimizations | Text     | Data |
| ----------- | ------------- | ---------| ---- |
| GCC 7.3.0   | -O2           | 7624     | 120  |
| GCC 7.3.0   | -Os           | 7296     | 120  |
| GCC 7.3.0   | -O2 -flto     | 5504     | 112  |
| GCC 7.3.0   | -Os -flto     | **5444** | 112  |
| GCC 8.2.0   | -O2           | 7600     | 120  |
| GCC 8.2.0   | -Os           | 7312     | 120  |
| GCC 8.2.0   | -O2 -flto     | 5520     | 112  |
| GCC 8.2.0   | -Os -flto     | 5448     | 112  |
| Clang 6.0.1 | -O2           | 7584     | 120  |
| Clang 6.0.1 | -Os           | 7516     | 120  |
| Clang 6.0.1 | -Oz           | **7264** | 120  |
| Clang 6.0.1 | -O2 -flto     | 6040     | 112  |
| Clang 6.0.1 | -Os -flto     | 5740     | 112  |
| Clang 6.0.1 | -Oz -flto     | 5596     | 112  |

### TYPE_TREE_MENU
| Compiler    | Optimizations | Text      | Data |
| ----------- | ------------- | --------- | ---- |
| GCC 7.3.0   | -O2           | 10552     | 124  |
| GCC 7.3.0   | -Os           | 10332     | 124  |
| GCC 7.3.0   | -O2 -flto     | 8128      | 116  |
| GCC 7.3.0   | -Os -flto     | 8112      | 116  |
| GCC 8.2.0   | -O2           | 10676     | 124  |
| GCC 8.2.0   | -Os           | 10344     | 124  |
| GCC 8.2.0   | -O2 -flto     | 8108      | 116  |
| GCC 8.2.0   | -Os -flto     | **8068**  | 116  |
| Clang 6.0.1 | -O2           | 10456     | 124  |
| Clang 6.0.1 | -Os           | 10292     | 124  |
| Clang 6.0.1 | -Oz           | **10116** | 124  |
| Clang 6.0.1 | -O2 -flto     | 8932      | 116  |
| Clang 6.0.1 | -Os -flto     | 8424      | 116  |
| Clang 6.0.1 | -Oz -flto     | 8240      | 116  |

### LUA
| Compiler    | Optimizations | Text       | Data |
| ----------- | ------------- | ---------- | ---- |
| GCC 7.3.0   | -O2           | 139344     | 584  |
| GCC 7.3.0   | -Os           | **127704** | 584  |
| GCC 7.3.0   | -O2 -flto     | 137824     | 580  |
| GCC 7.3.0   | -Os -flto     | **125768** | 580  |
| GCC 8.2.0   | -O2           | 140156     | 576  |
| GCC 8.2.0   | -Os           | 128460     | 576  |
| GCC 8.2.0   | -O2 -flto     | 138444     | 580  |
| GCC 8.2.0   | -Os -flto     | 126612     | 580  |
| Clang 6.0.1 | -O2           | 169728     | 580  |
| Clang 6.0.1 | -Os           | 137184     | 580  |
| Clang 6.0.1 | -Oz           | 132208     | 580  |
| Clang 6.0.1 | -O2 -flto     | 255728     | 576  |
| Clang 6.0.1 | -Os -flto     | 144600     | 576  |
| Clang 6.0.1 | -Oz -flto     | 132084     | 576  |


# Building 

### Prerequisites
The following packages must be installed in order to build the examples:
- arm-none-eabi-gcc ~~7.3.0.1~~ 8.2.0
- arm-none-eabi-newlib ~~3.0.0.1~~ 3.0.0.2
- arm-none-eabi-binutils 2.31.1
- clang 6.0.1-1

In case your installation of _arm-none-eabi_ does not default to _/usr/arm-none-eabi_ please make sure to adjust the paths in _mk/gcc-arm.mk_ and _mk/clang-arm.mk_. The toolchains can be switched by passing the additional symbol _CLANG_ to make.

So this runs the GCC build:
```
make
```

And this runs the Clang build:
```
make CLANG=1
```

Both, GCC and Clang, make use of the libstdc++ implementation from the _arm_none_eabi_ package.


