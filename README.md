### ★ 펌웨어
####    : 변경할 필요가 없는 소프트웨어를 ROM등에 고정시켜 하드웨어처럼 쓰는 것

### ★ LED
####    - 양극(+)[긴다리] : 애노드(Anode)
####    - 음극(-)[짧은다리] : 캐소드(Cathode)
####    - 허용전압 : 1.7V
####    - 허용전류 : 10mA
    
### ★ 옴의 법칙Ω
####    - 전압(V) = 전류(I) x 저항(R)

### ★ PWM (Pulse Width Modulation : 펄스 폭 변조)
####    - 순수 아날로그 제어를 모방한 것
####    - PWM을 사용하면 디지털 신호를 아날로그 신호와 비슷하게 만들 수 있음.
####    - PWM은 신호를 켜거나 끄는 방식으로 네모파의 듀티비를 변조하여 사용함.
####    - 듀티비(Duty Cycle) : 전체 주기에서 HIGH 상태가 차지하는 비율

### ★ 플로팅 현상
####    - LOW와 HIGH를 왔다갔다 하는 현상
####    - LOW 또는 HIGH 고정시켜야 함.
####    - LOW에 고정시키는 것을 풀다운 저항
####      : 버튼을 누르면 전류는 저항이 거의 없는 5V 경로를 따라 흐른다.
####        버튼을 누르지 않으면 전류는 저항이 높은 GND 경로를 따라 흐른다.
####    - HIGH에 고정시키는 것을 풀업 저항

### ★ 스위치 바운싱(채터링 또는 진동 잡음)
####    - 버튼의 기계적인 현상
####    - 값이 변경되는 것을 알아내기 어렵다.
####    - 실제로 버튼을 눌러도 예상대로 작동하지 않는 경우가 많다.

### ★ 디바운스 하는 방법
####    - 이전 버튼 상태와 현재 버튼 상태를 저장
####    - 현재 버튼 상태를 읽어 온다.
####    - 입력값이 이전 버튼 상태와 다를 때는 바운싱이 끝날때까지 5ms 동안 기다린다.
####    - 5ms 후에 버튼 상태를 다시 읽어서 그 값을 현재 버튼 상태로 저장한다.
####    - 이전 버튼 상태가 LOW이고 현재 버튼 상태가 HIGH라면 LED 상태를 전환한다.
####    - 이전 버튼 상태의 변수값에 현재 버튼 상태를 입력한다.
####    - 반복

### ★ PhotoResiter
####    - 주위가 밝으면 조도센서의 저항값이 줄어들면서 많은 전류가 흐르게 된다.
####    - 주위가 어두우면 저항값이 늘어나면서 적은 전류가 흐르게 된다.

### ★ Ultra sonic sensor
####    - 사물간의 거리를 인식하는 센서
####    - 초음파 속도 340 m/s(소리속도)  = 34000 cm/s    
####        => 1cm 가는데 걸리는 시간 = 1/34000 sec = 0.0000294118 = 29.4118 ㎲
####        => 1cm 를 갔다가 돌아오는데 걸리는 시간 = 58.8236 ㎲
####    - 거리(d) = 시간(t)*속도(m/s)
####    - 거리(cm) = 측정시간(㎲) / 58.8236(㎲/cm)

### ★ Joystick
####    - GND : GND
####    - 5V : 5V
####    - VRX : X축 중간값 약 512 A0
####    - VRY : Y축 중간값 약 512 A1
####    - SW : 누름 스위치 Normal ON(평상시 ON된 상태) Digital 3

### ★ 온습도센서 (파란색 PCB형 DHT-11)
####    - - : GND
####    - 가운데 : 5V
####    - S : 시그널 A0
####    - 라이브러리 : DHT.h

### ★ 7Segment
####    - Common-Anode Type : Common에 Vcc(5V) 가 연결되고 각 핀에 0V가 인가되면 LED가 들어오는 형태
####    - Common-Cathode Type : Common에 GND(0V)가 연결되고 각 핀이 VCC(5V)가 인가되면 LED가 들어오는 형태

### ★ LCD Display with Original
####    - 라이브러리 : LiquidCrystal
####    - GND : 0V
####    - VCC : 5V
####    - RST : 12
####    - EN : 11
####    - DB0~DB7 : Digital 5,4,3,2
####    - RW : 쓰기
####    - LED : BackLight

### ★ LCD Display with MH Module
####    - 라이브러리 : LiquidCrystal_I2C
####    - GND : 0V
####    - VCC : 5V
####    - SDA : A4 or pin27
####    - SCL : A5 or pin28

### ★ RFID Access
####    - RFID 태그
####    - RFID Reader
####    - 원리 : 자기장을 발생시켜 태그를 판독기에 가까이 가져가면 태그의 코일에서 유도전류 발생 이 전류를 통해 태그에 내장된 칩에 전원이 공급되고, 태그의 정보가 리더의 안테나로 전송된다. 결과적으로 리더는 태그 정보를 받아 대상을 식별합니다.
####    - 라이브러리 : rfid-master
####    -pin RST(Reset power Down Pin) : 9
####    -pin SDA(I2C-bus serial data line input/output) : 10
####    -pin MOSI(SPI master out, slave in) : 11
####    -pin MISO(SPI master in, slave out) : 12
####    -pin IRQ(interrupt request output: indicates an interrupt event) : Not Used
####    -pin SCK(SPI serial clock input) : 13
####    -pin GND : 0V
####    -pin 3.3V : 3.3V
####    카드 20 4 2 43 
####    둥근키 23 69 117 123  

### ★ STEPPING MOTOR
####    - IN1~IN4 : Digital Input
####    - + : 5V
####    - - : GND
####    - 라이브러리 : Stepper.h

### ★ GAS DETECT Sensor
####    - GND : GND
####    - VCC : 5V
####    - DO : Digital output
####    - AO : Analog output

### ★ Infrared DETECT Sensor
####    - GND : GND
####    - OUT : Digital output
####    - VCC : 5V
