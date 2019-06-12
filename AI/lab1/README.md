# Отчет по лабораторной работе
## по курсу "Искусственый интеллект"

### Студенты: 

| ФИО       | Роль в проекте                     | Оценка       |
|-----------|------------------------------------|--------------|
| Безлуцкая Е.Н. | Описание общей схемы предметной области, заполнение базы данных, обработка ответов  |          |
| Валентинова Ю.О.| Инженер по знаниям, заполнение базы данных, механизм задания вопросов  |       |
| Довженко А.А.| Тестирование,заполнение базы данных, механизм задания вопросов |      |
| Слесарева Д.С. | Написание отчёта, заполнение базы данных, менханизм вывода |          |

## Результат проверки

| Преподаватель     | Дата         |  Оценка       |
|-------------------|--------------|---------------|
| Сошников Д.В. |              |               |

> *Комментарии проверяющих (обратите внимание, что более подробные комментарии возможны непосредственно в репозитории по тексту программы)*

## Тема работы

На лекции по экспертным системам, всем командам было предложено набросать схему будущей ЭС. Наша команда решила пойти в сторону рекомендаций еды. Изначчально мы планировали рекомендовать блюда в зависимости от витаминов в их составе, но мы решили отказаться от этой идеи, так как было бы сложно хорошо продумать и написать это. Следующая, пришедшая к нам, идея - идея рекомендовать породу котов. Но, поразмыслив, мы поняли, что это тогда уже получится не рекомендательная система, а просто тест под названием "Выбери себе кота". После этого мы решили вернуться к теме еды и остановиться на одном блюде, а именно - на салатах. Пользователь отвечает на некоторые вопросы (вкус, соус, степень голода итд), и в результате, на основе его ответов, экспертная система рекомендует пользователю салат .

## Концептуализация предметной области

Результаты концептуализации предметной области:
 - выделенные понятия (Кухня, бюджет, вкус, вегетарианство, степень голода, соус, где пользователь желает покушать)
 - тип получившейся онтологии - иерархическое (фреймовое представление)
 - статические - кухня, бюджет, вкус, вегетарианство, степень голода, соус, где пользователь желает покушать
 - каждый участник выбрал кухню, салаты из которой он хотел искать (европейская была разделена на двоих участников), далее каждый занимался поиском информации и преобразованием её в предикаты

Приведите графические иллюстрации:
![Концептуализация](img/Expert.jpg)

## Принцип реализации системы

Для решения поставленной задачи используется система программирования SWI Prolog.
У нас есть небольшая база данных салатов. Пользователю рекомендуется вид салата исходя из его предпочтений, которые мы узнаём путем ответа пользователя на некоторые вопросы. Например, какой тип кухни хочет выбрать пользователь (европейская, азиатская, славянская), какой бюджет у пользователя ($, $$, $$$, $$$$, $$$$$), какой вкус хочет пользователь (сладкий, кислый, острый, нейтральный), является ли пользователь веритарианцем, или нет, голодный ли пользователь (по трехбальной шкале, где 1 - min, а 3 - max), какой соус хочет пользователь (майонез, бальзамический, оливковой масло, подсолнечное масло, мёд, горчица, лимонный, сметана, соевый или всё равно) и где пользователь хочет покушать (дома, в ресторане). Отверы обрабатываются, и в результате отсекаются некоторые ветки решения (если пользователь вегетарианец, то отсекается ветка решения с салатами с мясом).


## Механизм вывода

Механизм вывода вопросов:
```
answers([], _).
answers([First|Rest], Index):-
    write(Index), write(' '), answer(First), nl,
    NextIndex is Index + 1,
    answers(Rest, NextIndex).
```    

Механизм запоминания ответа пользователя:
```
ask(Question, Answer, Choices) :-
    question(Question),
    answers(Choices, 0),
    read(Index),
    parse(Index, Choices, Response),
    asserta(progress(Question, Response)),
    Response = Answer.
```    

Пользователю задаются вопросы, в базу знаний с помощью предиката asserta подгружаются новые ответы пользователя,когда какое-либо решение было найдено система отвечает на вопрос.

## Извлечение знаний и база знаний

Извлечение знаний происходило в ресторане "Обед буфет", мы несколько раз перерисовывали нашу сеть.

```
В коде за извлечение знаний отвечает данный фрагмент:
parse(0, [First|_], First).
parse(Index, [First|Rest], Response) :-
    Index > 0,
    NextIndex is Index - 1,
    parse(NextIndex, Rest, Response).
```

## Протокол работы системы

```
?- main.
Which salad should I eat now?
To answer, input the number of the answer

What kind of cuisine do you prefer?
0 European
1 Slavonian
2 Asiatic
|: 0.
What is your budget?
0 $
1 $$
2 $$$
3 $$$$
4 $$$$$
|: 2.
Do you want to eat at home or in a restaurant?
0 I want to cook at home
1 I want to eat in a restaurant
|: 1.
What taste do you prefer??
0 Sweet
1 Sour
2 Spicy
3 Neutral
|: 3.
Are you a vegetarian?
0 I'm a vegetarian
1 I'm not a vegetarian
|: 1.
How hungry are you?
0 Not really
1 Hungry
2 Very hungry
|: 1.
What kind of salad dressing do you prefer?
0 No matter
1 Mayonnaise
2 Olive oil
3 Sunflower oil
4 Honey
5 Mustard
6 Lemon
7 Balsamic
8 Cream
9 Soy
|: 1.
Ceasar with chicken.
You can find the reciept here:
https://eda.ru/recepty/salaty/salat-cezar-tradicionnij-25585
true.
?- resetAnswers.
true.
```

```
?- main.
Which salad should I eat now?
To answer, input the number of the answer

What kind of cuisine do you prefer?
0 European
1 Slavonian
2 Asiatic
|: 0.
What is your budget?
0 $
1 $$
2 $$$
3 $$$$
4 $$$$$
|: 4.
Do you want to eat at home or in a restaurant?
0 I want to cook at home
1 I want to eat in a restaurant
|: 0.
What taste do you prefer??
0 Sweet
1 Sour
2 Spicy
3 Neutral
|: 3.
Are you a vegetarian?
0 I'm a vegetarian
1 I'm not a vegetarian
|: 1.
How hungry are you?
0 Not really
1 Hungry
2 Very hungry
|: 2.
What kind of salad dressing do you prefer?
0 No matter
1 Mayonnaise
2 Olive oil
3 Sunflower oil
4 Honey
5 Mustard
6 Lemon
7 Balsamic
8 Cream
9 Soy
|: 0.
Salad with omar and truffels.
You can find the recipe here:
https://www.tveda.ru/recepty/salat-s-omarom/
true.
```

```
?- main.
Which salad should I eat now?
To answer, input the number of the answer

What kind of cuisine do you prefer?
0 European
1 Slavonian
2 Asiatic
|: 1.
What is your budget?
0 $
1 $$
2 $$$
3 $$$$
4 $$$$$
|: 1.
Do you want to eat at home or in a restaurant?
0 I want to cook at home
1 I want to eat in a restaurant
|: 0.
What taste do you prefer??
0 Sweet
1 Sour
2 Spicy
3 Neutral
|: 3.
Are you a vegetarian?
0 I'm a vegetarian
1 I'm not a vegetarian
|: 1.
How hungry are you?
0 Not really
1 Hungry
2 Very hungry
|: 2.
What kind of salad dressing do you prefer?
0 No matter
1 Mayonnaise
2 Olive oil
3 Sunflower oil
4 Honey
5 Mustard
6 Lemon
7 Balsamic
8 Cream
9 Soy
|: 1.
Mimosa salad.
You can find the reciept here:
https://eda.ru/recepty/salaty/klassicheskij-salat-mimoza-16807
true.
```


## Выводы

Лабораторная работа научила нас слаженной работе в команде. Мы уже не в первый раз выполняем проекты в институте в таком составе. Это позволило нам грамотно распределить задания между участниками команды, так как мы заранее знали сильные и слабые стороны всех членов команды. Для ускорения процесса разработки мы несколько раз встречались на нейтральной территории для работы на ЭС и пользовались давно созданным чатом для общения.
Мы очень долго и тщательно прорабатывали тему и концепцию нашей эксппертной системы, вместе освежали в памяти курс логического программирования. Концепция создания экспертной системы нам очень понравилась, это намного облегчает жизнь людям, мы даже задумывались о создании экспертной системы по теме ремонта, но это пока для нас было бы сложно. 
В будущем нам очень пригодятся знания о создании экспертных систем, потому что это довольно трендовое направление разработки, возможо, мы даже создадим свой большой проект.

