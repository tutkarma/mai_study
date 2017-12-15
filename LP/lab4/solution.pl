prefix("без").
prefix("бес").
prefix("вз").
prefix("вс").
prefix("вне").
prefix("воз").
prefix("вос").
prefix("все").
prefix("вы").
prefix("до").
prefix("за").
prefix("из").
prefix("ис").
prefix("к").
prefix("на").
prefix("не").
prefix("ни").
prefix("об").
prefix("от").
prefix("па").
prefix("пере").
prefix("под").
prefix("пре").
prefix("при").
prefix("про").
prefix("раз").
prefix("рас").
prefix("с").
prefix("тре").

suffix("онн").
suffix("ова").
suffix("ева").
suffix("ыва").
suffix("ива").
suffix("ен").
suffix("ан").

ending("у", genus("НЕТ"), num("ед")).
ending("ю", genus("НЕТ"), num("ед")).
ending("ет", genus("НЕТ"), num("ед")).
ending("ит", genus("НЕТ"), num("ед")).
ending("ют", genus("НЕТ"), num("мн")).
ending("ут", genus("НЕТ"), num("мн")).
ending("ат", genus("НЕТ"), num("мн")).
ending("ят", genus("НЕТ"), num("мн")).
ending("ете", genus("НЕТ"), num("мн")).
ending("ите", genus("НЕТ"), num("мн")).
ending("ем", genus("НЕТ"), num("мн")).
ending("им", genus("НЕТ"), num("мн")).
ending("ешь", genus("НЕТ"), num("ед")).
ending("ишь", genus("НЕТ"), num("ед")).
ending("ил", genus("М"), num("ед")).
ending("ел", genus("М"), num("ед")).
ending("ал", genus("М"), num("ед")).
ending("ела", genus("Ж"), num("ед")).
ending("ила", genus("Ж"), num("ед")).
ending("ала", genus("Ж"), num("ед")).
ending("ило", genus("С"), num("ед")).
ending("или", genus("НЕТ"), num("мн")).

dellast([_],[]):-!.
dellast([X|T],[X|Y]):-dellast(T,Y).

find_preffix([_|[]], pref(""), _):-!.
find_preffix([H|Slovo], pref(H), Slovo):-
    prefix(H).
find_preffix([H, H1|Slovo], pref(Pref), OstSlova):-
    string_concat(H, H1, NewPref),
    append([NewPref], Slovo, NewSlovo),
    find_preffix(NewSlovo, pref(Pref), OstSlova).

find_suffix([_|[]], suf(""), _):-!.
find_suffix(Slovo, suf(Suffix), Ost):-
    last(Slovo, Suffix),
    dellast(Slovo, Ost),
    suffix(Suffix).
find_suffix(Slovo, suf(S), OstSlova):-
    last(Slovo, Suffix),
    dellast(Slovo, Ost),
    last(Ost, Suffix2),
    dellast(Ost, Ost2),
    string_concat(Suffix2, Suffix, NewSuffix),
    append(Ost2, [NewSuffix], NewSlovo),
    find_suffix(NewSlovo, suf(S), OstSlova).

find_ending([_|[]], end(""), rod("М"), chislo("ед"), _):-!.
find_ending(Slovo, end(End), Rod, Chislo, Ost):-
    last(Slovo, End),
    dellast(Slovo, Ost),
    ending(End, Rod, Chislo).
find_ending(Slovo, end(E), Rod, Chislo, OstSlova):-
    last(Slovo, End),
    dellast(Slovo, Ost),
    last(Ost, End2),
    dellast(Ost, Ost2),
    string_concat(End2, End, NewEnd),
    append(Ost2, [NewEnd], NewSlovo),
    find_ending(NewSlovo, end(E), Rod, Chislo, OstSlova).

analiz(Slovo, morf(pref(Y), root(Ost), end(Z), Rod, Chislo)):-
    find_preffix(Slovo, pref(Y), Slovo1), !,
    (Y == "" -> Slovo1 = Slovo; Slovo1 = Slovo1),
    find_ending(Slovo1, end(Z), Rod, Chislo, Slovo2), !,
    (Z == "" -> Slovo2 = Slovo1; Slovo2 = Slovo2),
    find_suffix(Slovo2, suf(X), Ost), !,
    (X == "" -> Ost = Slovo2; Ost = Ost).
