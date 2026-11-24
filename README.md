# SALI: sistema de aspersão localizada para incêndio

Projeto da disciplina Oficina de Integração 1 (UTFPR), desenvolvido em equipe.

Uma câmera térmica MLX90640 (matriz de 32x24 pixels) procura um foco de calor. Quando encontra, o ESP32 envia a posição do pixel mais quente para um Arduino, que calcula o ângulo de disparo do jato d'água por balística, aponta dois servos (horizontal e vertical) e aciona a bomba e a válvula solenoide.

## Estrutura

```
leitura_mlx90640/leitura_mlx90640.ino   ESP32: lê a câmera e envia a posição do foco pela serial
bomba_valvula/bomba_valvula.ino         Arduino: calcula os ângulos, move os servos e aciona bomba e válvula
```

O sketch da câmera parte do exemplo `Example1_BasicReadings` da SparkFun (licença MIT), adaptado para mandar a posição do foco por uma segunda serial.

## Funcionamento

1. O ESP32 lê os dois subquadros da MLX90640 e guarda o pixel mais quente; se ele passa do limiar (`TRESHOLD`), considera que há fogo.
2. A posição é enviada como um inteiro `I*100 + J` pela serial (pinos 16 e 17).
3. O Arduino converte linha e coluna em distâncias sobre a mesa e encontra, por busca iterativa, o ângulo de elevação que faz o jato (o código usa velocidade de saída de 1,7 m/s) atingir o ponto.
4. Os servos giram até o ângulo calculado, e a bomba (relé no pino 7) e a válvula (relé no pino 8) são ligadas.

## Tecnologias

ESP32 e Arduino (C++/Arduino), câmera térmica MLX90640 por I2C, servos, relés, bomba d'água e válvula solenoide.

## Como gravar

1. ESP32: instale a biblioteca da Melexis para a MLX90640 (arquivos `MLX90640_API` e `MLX90640_I2C_Driver`) e grave `leitura_mlx90640.ino`.
2. Arduino: grave `bomba_valvula.ino` (biblioteca Servo, já incluída na IDE).
3. Ligue o TX do ESP32 (pino 17) ao RX do Arduino, compartilhando o GND.
