% получение элемента списка
getNthElem([H|_],H,0):-!.
getNthElem([_|T],X,N):-
    M is N - 1,
    getNthElem(T,X,M).

% свапнуть два элемента
swap_elem(L,M,N,ResL):-
    length(L,Len1),
    length(ResL,Len2),
    Len1 =:= Len2,
    append(MH,[Mth|MT],L),
    append(MH,[Nth|MT],Tmp),
    append(NH,[Nth|NT],Tmp),
    append(NH,[Mth|NT],ResL),

    length(MH,M),
    length(NH,N), !.

% элементы в правильном порядке
check_correct(L,N,M):-
    getNthElem(L,X,N),
    getNthElem(L,Y,M),
    ((X == blue, Y == red);
    (X == blue, Y == white);
    (X == white, Y == red)),
    !.

% переход между состояниями
move([H|_],Res):-
    move(H,Res).

move(st(L),st(ResL)):-
    length(L,Len),
    Len1 is Len - 1,
    between(0,Len1,A),
    between(0,Len1,B),

    A @< B,
    check_correct(L,A,B),
    swap_elem(L,A,B,X),

    ResL = X.

% продление пути без зацикливания
prolong([X|T],[Y,X|T]):-
    move(X,Y),
    \+ member(Y,[X|T]).

inv_print([]).
inv_print([A|T]):-inv_print(T), write(A), nl.

int(1).
int(X):-
    int(Y),
    X is Y + 1.

% поиск в глубину
search_dpth(A,B):-
    write('DFS START'), nl,
    get_time(DFS),
    dpth([A],B,L),
    inv_print(L),
    get_time(DFS1),
    write('DFS END'), nl, nl,
    T1 is DFS1 - DFS,
    write('TIME IS '), write(T1), nl, nl.

dpth([X|T],X,[X|T]).
dpth(P,F,L):-
    prolong(P,P1),
    dpth(P1,F,L).

% поиск в ширину
search_bdth(X,Y):-
    write('BFS START'), nl,
    get_time(BFS),
    bdth([[X]],Y,L),
    inv_print(L),
    get_time(BFS1),
    write('BFS END'), nl, nl,
    T1 is BFS1 - BFS,
    write('TIME IS '), write(T1), nl, nl.

bdth([[X|T]|_],X,[X|T]).
bdth([P|QI],X,R):-
    findall(Z,prolong(P,Z),T),
    append(QI,T,Q0),
    bdth(Q0,X,R).

bdth([_|T],Y,L):-
    bdth(T,Y,L).

% поиск с итерационным заглублением
search_id(Start,Finish):-
    write('ITER START'), nl,
    get_time(ITER),
    int(DepthLimit),
    depth_id([Start],Finish,Res,DepthLimit),
    inv_print(Res),
    get_time(ITER1),
    write('ITER END'), nl, nl,
    T1 is ITER1 - ITER,
    write('TIME IS '), write(T1), nl, nl.

depth_id([Finish|T],Finish,[Finish|T],0).
depth_id(Path,Finish,R,N):-
    N @> 0,
    prolong(Path,NewPath),
    N1 is N - 1,
    depth_id(NewPath,Finish,R,N1).

search_id(Start,Finish,Path):-
    int(Level),
    search_id(Start,Finish,Path,Level).
