### Лабораторная 2 "Семисегментный индикатор"

С внутренним устройством семисегментного индикатора можно ознакомиться в датащите 7segm.h. Если коротко, то устройство имеет 10 выводов, центральный вывод в каждом ряду это общий анод/катод в зависимости от типа индикатора. Остальные выводы подключаются непосредственно к каждому из сегментов a, b, c, d, e, f, g, dp(точка).

Рассмотим подключение индикатора на примере функции SetSegm из файла lib/device-drivers/7segm.h. Для удобства рекомендуется сделать переопределение пинов в соответствии с тем, как вы их подключаете. В приведенной реализации это сделано следующим образом:

```C
#define DIOD_A PIN_11
#define DIOD_B PIN_7
#define DIOD_C PIN_4
#define DIOD_D PIN_2
#define DIOD_E PIN_1
#define DIOD_F PIN_10
#define DIOD_G PIN_5
#define DIOD_DP PIN_3
#define DIGIT1 PIN_12
#define DIGIT2 PIN_9
#define DIGIT3 PIN_8
#define DIGIT4 PIN_6
```

Если сделать подобную цепочку редефайнов, то при замене пинов, к которым подключен индикатор, достаточно будет поменять только одну строчку, вместо того, чтобы бегать по всем написаным функциям и заменять их там. В данных дефайнах все обозначения сделаны в соответствии с датащитом 7segm.h.

Теперь рассомтрим, что происходит в функции SetSegm:
Включаем тактирование на выбранном порте:

```C
PortX_EnableClock(port);

// текст функции PortX_EnableClock(port) (lib/config/config.h)

// По сути, это вызов системной функции включения тактирования от всех возможных переменных.
// Существование такой функции обусловлено параметрической инициализацией всей внешней
// периферии в данной релизации
int PortX_EnableClock(GPIO_TypeDef* port) {
  if (port == GPIOA) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  } else if (port == GPIOB) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  } else if (port == GPIOC) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  } else if (port == GPIOD) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
  } else if (port == GPIOF) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  } else {
    return -1;
  }

  return 0;
}
```

Далее, выставляем все нужные пины в режим цифрового выхода:

```C
  LL_GPIO_SetPinMode(port, DIGIT1, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIGIT2, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIGIT3, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIGIT4, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIOD_A, LL_GPIO_MODE_OUTPUT);
  // ...
  LL_GPIO_SetPinMode(port, DIOD_G, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIOD_DP, LL_GPIO_MODE_OUTPUT);
```

Теперь рассмотрим вывод чего-либо на индикатор. В общем случае, нам было бы необходио вызвать функцию LL_GPIO_WriteOutputPort(port, 0x...), однако сначала надо получить значение, которое мы будем писать в порт, а именно, все пины, которые нам необходимо активировать.

Короткий пример: чтобы отобразить цифру `1`, необходимо подать на порт X значение `0x0006` (нужно помнить, что 7 выход порта должен быть в нуле для индикатора с общим катодом, и всегда в единице для индикатора с общим анодом). Убедитесь в этом (не забудьте подать тактирование на порт X):

```C
// Вариант для случая, если индикатор с общим катодом
LL_GPIO_WriteOutputPort(GPIOX, 0x0006);
// Вот так выглядела бы запись для индикатора с общим анодом
LL_GPIO_WriteOutputPort(GPIOX, 0xFF & ~0x0006);
```

Чтобы не высчитывать, какое значение соответствует каждому из символов, можно сделать тематические константные массивы, как это было сделано с числами в lib/device-drivers/7segm.h:

```C
const unsigned int SEGM_NUMBERS[] = {
    DIOD_A | DIOD_B | DIOD_C | DIOD_D | DIOD_E | DIOD_F,          // 0
    DIOD_B | DIOD_C,                                              // 1
    // ...
    DIOD_A | DIOD_G | DIOD_B | DIOD_C | DIOD_D | DIOD_F           // 9
};
```

Для того, чтобы задействовать все ячейки индикатора, нам необходим некий цикл, в котором мы будем поочередно выставлять сначала DIGIT1 - в 0, DIGIT2, DIGIT3, DIGIT4 - в 1, затем DIGIT1 - в 1, DIGIT2 - в 0, DIGIT3, DIGIT4 - в 1, и т.д. (т.е. поочередно создавать разность потенциалов только на той ячейке, в которой мы хотим отображать символ)

В качестве упражнения можете написать массив для буков и программу, которая считает от 0 до f, инкрементируя значение при нажатии кнопки. Если у вас возникнут трудности, изучите функцию Segm_SetNum2 в lib/device-drivers/7segm.h.

## main.c

В файле main.c можно найти возможную реализацию алгоритмов отображения статического числа, а также инкрементирующегося от нажатия кнопки
