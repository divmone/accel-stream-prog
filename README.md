# accel-stream-prog

Реализован вариант 2 (Linux, альтернативный) уровень 1: TCP-сокеты + JSON.

Три процесса обмениваются пакетами по TCP. КлиентА генерирует синусоидальный сигнал ускорения (~50 Гц), сервер пересылает его КлиентуБ, тот считает модуль вектора и отправляет результат обратно через сервер КлиентуА, который пишет значения в файл.

---

## Как собрать?
```bash
cmake -B build
cmake --build build -j
```

По умолчанию Boost подключается через `find_package` (требуется установленный `libboost-system-dev`). Если Boost не установлен в системе, можно собрать с опцией `USE_FETCH_BOOST=ON` - тогда Boost будет скачан через `FetchContent` :

```bash
cmake -B build -DUSE_FETCH_BOOST=ON
cmake --build build -j
```

---

## Запуск

```bash
./build/server/server
./build/clientB/clientB
./build/clientA/clientA
```

- Сервер слушает порты `9000` (для КлиентаА) и `9001` (для КлиентаБ) на `127.0.0.1`.
- КлиентА пишет полученные модули в `logs/accel_module.log`.
- Остановка по `Ctrl+C` (`SIGINT`).

---

## Протокол

**AccelPacket**:
```json
{"timestamp": 1715600000000, "x": 0.12, "y": -0.84, "z": 0.55}
```

**ModulePacket** :
```json
{"timestamp": 1715600000000, "module": 1.024}
```

---

## Архитектура

Поток данных:
1. КлиентА генерирует пакет и шлёт на сервер.
2. Сервер читает пакет, отбрасывает подряд идущий дубликат и пересылает на КлиентБ.
3. КлиентБ считает модуль вектора (`accel::calcModule`), формирует `ModulePacket` и шлёт обратно.
4. Сервер пересылает пакет назад в КлиентА.
5. КлиентА пишет результат в `logs/accel_module.log` через `ModuleLogger`.

### Общая часть (`core/`)
- **`Protocol`** - структуры `AccelPacket` и `ModulePacket` с `toJSON`/`fromJSON`/`fromString`.
- **`TcpConnection`** - сокет, асинхронные чтение/запись.
- **`TcpClient::connect`** - подключение по IP и порту, возвращает `TcpConnection`.
- **`CoroRuntime`**:
  - `run()` - запускает `io_context` с обработкой `SIGINT`;
  - `runTcpClient()` - цикл переподключения для клиентов (любая ошибка -> ждём 1 секунду -> реконнект).

### Сервер (`server/`)
- **`AsyncServer`** - принимает пары (соединение А, соединение Б) и запускает `Session`.
- **`Session`** - две forward-корутины, объединённые через `co_await (A->B || B->A)`. Если одна падает - вторая отменяется, сессия закрывается, сервер принимает новую пару.

### Клиенты (`clientA/`, `clientB/`)
- `reader` и `writer` внутри одного процесса тоже объединены через `||`.
- При любой ошибке `runTcpClient` ловит исключение, ждёт 1 секунду и переподключается.

---

Для работы с сетью используется библиотека Boost.Asio.
Для логгирования используется библиотека spdlog.

## Зависимости
- [nlohmann/json](https://github.com/nlohmann/json/) - через FetchContent.
- [spdlog](https://github.com/gabime/spdlog) - через FetchContent.
- Boost (`system` + `asio`) - через `find_package` (по умолчанию) или `FetchContent` при `-DUSE_FETCH_BOOST=ON`.
