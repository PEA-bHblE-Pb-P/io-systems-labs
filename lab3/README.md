# Лабораторная работа 3

**Название:** "Разработка драйверов сетевых устройств"

**Цель работы:** получить знания и навыки разработки драйверов сетевых интерфейсов для операционной системы Linux.

## Описание функциональности драйвера

- Осуществляет перехват пакетов протокола UDP, адресуемых на конкретный порт
- Выводит порты отправителя и получателя, передаваемые данные
- Состояние разбора пакетов выводится в файл /proc/var3

## Инструкция по сборке

Сборка модуля:

```
make
```

Сборка с последующей установкой:

```
make install
```

Отключение модуля:

```
make uninstall
```

Очистка артефактов сборки:

```
make clean
```

## Инструкция пользователя

Отправка udp пакета:
```bash
nc -u 127.0.0.1 4848
test
```

Чтение результата:
```bash
cat /proc/var3
```

## Примеры использования

Отправляем 3 udp пакета на выбранный порт (4848):
```console
nc -u 127.0.0.1 4848
test
test2
test3
```

Проверяем:
```console
dmesg
[ 1190.275659] Module lab3 loaded
[ 1190.275662] lab3: create link vni0
[ 1190.275663] lab3: registered rx handler for lo
[ 1190.289681] vni0: device opened
[ 1195.215461] Captured UDP datagram, saddr: 127.0.0.1
[ 1195.215472] daddr: 127.0.0.1
[ 1195.215476] Captured IP packet, sport: 58365
[ 1195.215479] dport: 4848
[ 1195.215481] sport: 58365
[ 1195.215484] Data length: 5
[ 1195.215486] Data: test

[ 1195.215534] [UFW BLOCK] IN=vni0 OUT= MAC=00:00:00:00:00:00:00:00:00:00:00:00:08:00 SRC=127.0.0.1 DST=127.0.0.1 LEN=33 TOS=0x00 PREC=0x00 TTL=64 ID=17013 DF PROTO=UDP SPT=58365 DPT=4848 LEN=13 
[ 1201.006408] [UFW BLOCK] IN=vni0 OUT= MAC=00:00:00:00:00:00:00:00:00:00:00:00:08:00 SRC=127.0.0.1 DST=127.0.0.53 LEN=85 TOS=0x00 PREC=0x00 TTL=64 ID=55710 DF PROTO=UDP SPT=36358 DPT=53 LEN=65 
[ 1205.103534] Captured UDP datagram, saddr: 127.0.0.1
[ 1205.103544] daddr: 127.0.0.1
[ 1205.103548] Captured IP packet, sport: 58365
[ 1205.103552] dport: 4848
[ 1205.103555] sport: 58365
[ 1205.103557] Data length: 6
[ 1205.103559] Data: test2

[ 1205.103613] [UFW BLOCK] IN=vni0 OUT= MAC=00:00:00:00:00:00:00:00:00:00:00:00:08:00 SRC=127.0.0.1 DST=127.0.0.1 LEN=34 TOS=0x00 PREC=0x00 TTL=64 ID=17014 DF PROTO=UDP SPT=58365 DPT=4848 LEN=14 
[ 1216.020292] [UFW BLOCK] IN=vni0 OUT= MAC=00:00:00:00:00:00:00:00:00:00:00:00:08:00 SRC=127.0.0.1 DST=127.0.0.53 LEN=129 TOS=0x00 PREC=0x00 TTL=64 ID=12629 DF PROTO=UDP SPT=39348 DPT=53 LEN=109 
[ 1237.319047] Captured UDP datagram, saddr: 127.0.0.1
[ 1237.319059] daddr: 127.0.0.1
[ 1237.319063] Captured IP packet, sport: 58365
[ 1237.319066] dport: 4848
[ 1237.319069] sport: 58365
[ 1237.319071] Data length: 6
[ 1237.319073] Data: test3

[ 1237.319129] [UFW BLOCK] IN=vni0 OUT= MAC=00:00:00:00:00:00:00:00:00:00:00:00:08:00 SRC=127.0.0.1 DST=127.0.0.1 LEN=34 TOS=0x00 PREC=0x00 TTL=64 ID=17015 DF PROTO=UDP SPT=58365 DPT=4848 LEN=14
```

В файле записано, что было обработано 3 пакета:
```console
cat /proc/var3
Processed: 3; Dropped: 4
```
