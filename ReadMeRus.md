# Simple vector

## Описание

`SimpleVector` - eпрощенная реализация `std::vector` на C++17(STL).<br><br>
`SimpleVector` характеризуется параметрами *Размер* и *Вместимость*. Данные хранятся в `куче`, в самом `SimpleVector` хранится только адрес первого элемента массива или `nullptr`.<br>
 Реализованы конструкторы `SimpleVector`: по умолчанию, копирования, перемещения и парметризованные конструкторы для задания размера, вместимости и задания содержимого массива.<br>
 Для `SimpleVector` разработаны методы и операторы аналогичные по функционалу методам и операторам `std::vector`.<br>
 Методы: `GetSize`, `GetCapacity`, `IsEmpty`, `PushBack`, `PopBack`, `Insert`, `Erase`, `swap`, `Reserve`, `At`, `Clear`, `Resize`, `begin`, `end`, `cbegin` и `cend`.<br>
 Операторы: `[]`, `==`, `!=`, `<`, `>`, `<=` и `>=`.

## Использованный стек технологий
  - Copy elision
  - NRVO ( Named Return Value Optimization )
  - move - семантика
  - rvalue-ссылки
