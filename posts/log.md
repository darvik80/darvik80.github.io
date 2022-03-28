## IoT for dev
### Log me baby one more time...
После выбора IDE надо продолжить работать над собой и задуматься, а что же можно улучшить в логгировании.
И в этом маленьком [проекте](https://github.com/darvik80/darvik80.github.io/tree/master/source/arduino-uno-logging) 
я попытался сделать этот процесс более простым и информативным, в качестве 
маленького бонуса так же давайте добавим веселые ~~обои~~ цвета.
<br>
* Инициализация Serial port и добавление логера
```cpp
    Serial.begin(115200);
    logging::CompositeLogger::instance().addLogger(new logging::SerialLogger());
```

* У нас есть модуль? Давайте заведем для него свой логгер - с блекджеком и куртизанками
```cpp
LOG_COMPONENT_SETUP(mqtt) 
```
* Ну а теперь давайте что-нибудь залогируем
```cpp
    logging::info("globalog"); 
```

```cpp
    mqtt::log::info("hello, i'm mqtt module"); 
```