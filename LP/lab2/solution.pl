% Вариант 7
% На одном международном конгрессе встретились 4 делегата из разных стран. Каждый из них владел двумя языками из 4 (английский, французский, немецкий,
% итальянский). Однако, оказалось, что не было такого языка, на котором они могли бы говорить вчетвером. И был только один язык, на котором могли 
% вести беседу трое из них. Никто из делегатов не владеет французским и немецким языками одновременно. Хотя физик не говорит по-английски, он может
% служить переводчиком, если математик и биолог захотят поговорить друг с другом. Биолог говорит по-немецки и может говорить с химиком, хотя тот
% не знает ни одного немецкого слова. Физик, математик и химик не могут беседовать втроем на одном языке. Какими двумя языками владеет каждый из них?

lan(english).
lan(french).
lan(german).
lan(italian).
del(Lan1, Lan2):-lan(Lan1), lan(Lan2).
speak(del(L, _), L).
speak(del(_, L), L).

common_lan([Del1, Del2, Del3, Del4]):-
    speak(Del1, L), speak(Del2, L), speak(Del3, L), speak(Del4, L).

lan_for_3([Del1, Del2, Del3,_], L):-
    speak(Del1, L), speak(Del2, L), speak(Del3, L).
lan_for_3([Del1, Del2,_, Del4], L):-
    speak(Del1, L), speak(Del2, L), speak(Del4, L).
lan_for_3([Del1,_, Del3, Del4], L):-
    speak(Del1, L), speak(Del3, L), speak(Del4, L).
lan_for_3([_, Del2, Del3, Del4], L):-
    speak(Del2, L), speak(Del3, L), speak(Del4, L).

lan_for_3_optional([Del1, Del2, Del3, Del4], L):-
    lan_for_3([Del1, Del2, Del3, Del4], L1), L1 \== L.

member(Del,[Del|_]).
member(Del,[_|Dels]):-member(Del, Dels).

math_and_bio_speaks(Math, Bio):-
    speak(Math, L), speak(Bio, L).

phys_math_chem_speaks(Phys, Math, Chem):-
    speak(Phys, L), speak(Math, L), speak(Chem, L).

main([Phys, Math, Bio, Chem]):-
    lan(Lan11), lan(Lan12), Lan11 @< Lan12,
    Phys = del(Lan11, Lan12),
    lan(Lan21), lan(Lan22), Lan21 @< Lan22,
    Math = del(Lan21, Lan22),
    lan(Lan31), lan(Lan32), Lan31 @< Lan32,
    Bio = del(Lan31,Lan32),
    lan(Lan41), lan(Lan42), Lan41 @< Lan42,
    Chem = del(Lan41, Lan42),
    Dels = [Phys, Math, Bio, Chem],
    not(common_lan(Dels)),
    lan_for_3(Dels, L),
    not(lan_for_3_optional(Dels, L)),
    not(member(del(french,german), Dels)),
    not(speak(Phys,english)),
    speak(Phys, LFM), speak(Math, LFM),
    speak(Phys, LFB), speak(Bio, LFB),
    not(math_and_bio_speaks(Math, Bio)),
    speak(Bio,german),
    speak(Bio, LBH), speak(Chem, LBH),
    not(speak(Chem, german)),
    not(phys_math_chem_speaks(Phys, Math, Chem)).