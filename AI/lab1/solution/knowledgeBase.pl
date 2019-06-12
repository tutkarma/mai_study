salad(caesar):-
	cuisine(european),
	budget(three),
	eat(out),
	taste(neutral),
	isVegan(notveg),
	hunger(medium),
	sauceBase(mayonnaise).

salad(caesar):-
	cuisine(european),
	budget(three),
	eat(in),
	taste(neutral),
	isVegan(notveg),
	hunger(medium),
	sauceBase(mayonnaise).

salad(mimosa):-
	cuisine(slavonian),
	budget(two),
	eat(in),
	taste(neutral),
	isVegan(notveg),
	hunger(high),
	sauceBase(mayonnaise).

salad(carrotApple):-
	cuisine(slavonian),
	budget(one),
	eat(in),
	taste(sweet),
	isVegan(veg),
	hunger(high),
	sauceBase(honey).

salad(carrotApple):-
	cuisine(slavonian),
	budget(one),
	eat(in),
	taste(sweet),
	isVegan(notveg),
	hunger(high),
	sauceBase(honey).

salad(seledkaPodShuboj):-
	cuisine(slavonian),
	budget(two),
	eat(in),
	taste(neutral),
	isVegan(notveg),
	hunger(high),
	sauceBase(mayonnaise).

salad(vinegret):-
	cuisine(slavonian),
	budget(two),
	eat(in),
	taste(neutral),
	isVegan(veg),
	hunger(medium),
	sauceBase(sunflower).

salad(vinegret):-
	cuisine(slavonian),
	budget(two),
	eat(in),
	taste(neutral),
	isVegan(notveg),
	hunger(medium),
	sauceBase(sunflower).

salad(vinegret):-
	cuisine(slavonian),
	budget(two),
	eat(in),
	taste(sour),
	isVegan(veg),
	hunger(medium),
	sauceBase(sunflower).

salad(vinegret):-
	cuisine(slavonian),
	budget(two),
	eat(in),
	taste(sour),
	isVegan(notveg),
	hunger(medium),
	sauceBase(sunflower).

salad(olive):-
	cuisine(slavonian),
	budget(two),
	eat(in),
	taste(neutral),
	isVegan(notveg),
	hunger(high),
	sauceBase(mayonnaise).

salad(koreanEggplantSalad):-
	cuisine(asiatic),
	budget(two),
	eat(in),
	taste(spicy),
	isVegan(veg),
	hunger(high),
	sauceBase(vinegar).

salad(koreanEggplantSalad):-
	cuisine(asiatic),
	budget(two),
	eat(in),
	taste(spicy),
	isVegan(notveg),
	hunger(high),
	sauceBase(vinegar).

salad(shrimpSalad):-
	cuisine(asiatic),
	budget(three),
	eat(in),
	taste(sweet),
	isVegan(notveg),
	hunger(medium),
	sauceBase(olive).

salad(shrimpSalad):-
	cuisine(asiatic),
	budget(three),
	eat(in),
	taste(sour),
	isVegan(notveg),
	hunger(medium),
	sauceBase(olive).

salad(lotusHuaChen):-
	cuisine(asiatic),
	budget(four),
	eat(out),
	taste(spicy),
	isVegan(veg),
	hunger(medium),
	sauceBase(lemon).

salad(lotusHuaChen):-
	cuisine(asiatic),
	budget(four),
	eat(out),
	taste(spicy),
	isVegan(notveg),
	hunger(medium),
	sauceBase(lemon).
	
salad(caprese):-
	cuisine(european),
	budget(three),
	eat(in),
	taste(neutral),
	isVegan(veg),
	hunger(high),
	sauceBase(olive).

salad(caprese):-
	cuisine(european),
	budget(three),
	eat(in),
	taste(neutral),
	isVegan(notveg),
	hunger(high),
	sauceBase(olive).

salad(presto):-
	cuisine(european),
	budget(two),
	eat(out),
	taste(neutral),
	isVegan(veg),
	hunger(medium),
	sauceBase(olive).

salad(presto):-
	cuisine(european),
	budget(two),
	eat(out),
	taste(neutral),
	isVegan(notveg),
	hunger(medium),
	sauceBase(olive).
 
salad(tiger):-
	cuisine(european),
	budget(two),
	eat(out),
	taste(neutral),
	isVegan(veg),
	hunger(high),
	sauceBase(noMatter).

salad(tiger):-
	cuisine(european),
	budget(two),
	eat(out),
	taste(neutral),
	isVegan(notveg),
	hunger(high),
	sauceBase(noMatter).
 
salad(greek):-
	cuisine(european),
	budget(two),
	eat(out),
	taste(neutral),
 	isVegan(veg),
	hunger(medium),
	sauceBase(olive).

salad(greek):-
	cuisine(european),
	budget(two),
	eat(out),
	taste(neutral),
 	isVegan(notveg),
	hunger(medium),
	sauceBase(olive).
	
salad(potato):-
	cuisine(european),
	budget(two),
	eat(out),
	taste(sour),
	isVegan(notveg),
	hunger(high),
	sauceBase(mayonnaise).

salad(shrimpMusselsSalad):-
	cuisine(asiatic),
	budget(five),
	eat(out),
	taste(neutral),
	isVegan(veg),
	hunger(high),
	sauceBase(olive).

salad(shrimpMusselsSalad):-
	cuisine(asiatic),
	budget(five),
	eat(out),
	taste(neutral),
	isVegan(notveg),
	hunger(high),
	sauceBase(olive).

salad(nicoise):-
	cuisine(european),
	budget(four),
	eat(out),
	taste(neutral),
	isVegan(veg),
	hunger(medium),
	sauceBase(olive).

salad(nicoise):-
	cuisine(european),
	budget(four),
	eat(out),
	taste(neutral),
	isVegan(notveg),
	hunger(medium),
	sauceBase(olive).

salad(malaga):-
	cuisine(european),
	budget(two),
	eat(out),
	taste(sour),
	isVegan(veg),
	hunger(high),
	sauceBase(olive).

salad(malaga):-
	cuisine(european),
	budget(two),
	eat(out),
	taste(sour),
	isVegan(notveg),
	hunger(high),
	sauceBase(olive).

salad(shopska):-
	cuisine(european),
	budget(three),
	eat(out),
	taste(sour),
	isVegan(veg),
	hunger(medium),
	sauceBase(olive).

salad(shopska):-
	cuisine(european),
	budget(three),
	eat(out),
	taste(sour),
	isVegan(notveg),
	hunger(medium),
	sauceBase(olive).

salad(sashimiSalad):-
	cuisine(asiatic),
	budget(four),
	eat(in),
	taste(neutral),
	isVegan(veg),
	hunger(high),
	sauceBase(soy).

salad(sashimiSalad):-
	cuisine(asiatic),
	budget(four),
	eat(in),
	taste(neutral),
	isVegan(notveg),
	hunger(high),
	sauceBase(soy).

salad(rise):-
	cuisine(asiatic),
	budget(three),
	eat(in),
	taste(neutral),
	isVegan(notveg),
	hunger(high),
	sauceBase(soy).

salad(kayso):-
	cuisine(asiatic),
	budget(one),
	eat(out),
	taste(neutral),
	isVegan(veg),
	hunger(low),
	sauceBase(soy).

salad(kayso):-
	cuisine(asiatic),
	budget(one),
	eat(out),
	taste(neutral),
	isVegan(notveg),
	hunger(low),
	sauceBase(soy).

salad(brokenCucumbers):-
	cuisine(asiatic),
	budget(one),
	eat(in),
	taste(sour),
	isVegan(veg),
	hunger(low),
	sauceBase(olive).

salad(brokenCucumbers):-
	cuisine(asiatic),
	budget(one),
	eat(in),
	taste(sour),
	isVegan(veg),
	hunger(low),
	sauceBase(olive).

salad(danki):-
	cuisine(asiatic),
	budget(three),
	eat(out),
	taste(sour),
	isVegan(notveg),
	hunger(medium),
	sauceBase(balsamic).

salad(unagiSarada):-
	cuisine(asiatic),
	budget(two),
	eat(in),
	taste(sour),
	isVegan(veg),
	hunger(high),
	sauceBase(lemon).

salad(unagiSarada):-
	cuisine(asiatic),
	budget(two),
	eat(in),
	taste(sour),
	isVegan(notveg),
	hunger(high),
	sauceBase(lemon).

salad(goatsCheese):-
	cuisine(slavonian),
	budget(five),
	eat(out),
	taste(sweet),
	isVegan(veg),
	hunger(medium),
    sauceBase(honey).

salad(goatsCheese):-
	cuisine(slavonian),
	budget(five),
	eat(out),
	taste(sweet),
	isVegan(notveg),
	hunger(medium),
    sauceBase(honey).

salad(beef):-
	cuisine(slavonian),
	budget(five),
	eat(out),
	taste(sweet),
	isVegan(notveg),
	hunger(low),
    sauceBase(noMatter).

salad(meat):-
	cuisine(european),
	budget(four),
	eat(in),
	taste(spicy),
	isVegan(notveg),
	hunger(high),
	sauceBase(mustard).

salad(meatChili):-
	cuisine(european),
	budget(three),
	eat(in),
	taste(spicy),
	isVegan(notveg),
	hunger(medium),
	sauceBase(soy).

salad(threeBeans):-
	cuisine(slavonian),
	budget(one),
	eat(in),
	taste(neutral),
	isVegan(veg),
	hunger(low),
	sauceBase(sunflower).

salad(threeBeans):-
	cuisine(slavonian),
	budget(one),
	eat(in),
	taste(neutral),
	isVegan(notveg),
	hunger(low),
	sauceBase(sunflower).

salad(eggplants):-
	cuisine(european),
	budget(two),
	eat(out),
	taste(neutral),
	isVegan(veg),
	hunger(low),
	sauceBase(sunflower).

salad(eggplants):-
	cuisine(european),
	budget(two),
	eat(out),
	taste(neutral),
	isVegan(notveg),
	hunger(low),
	sauceBase(sunflower).

salad(breeze):-
	cuisine(slavonian),
	budget(two),
	eat(out),
	taste(neutral),
	isVegan(veg),
	hunger(medium),
	sauceBase(balsamic).

salad(breeze):-
	cuisine(slavonian),
	budget(two),
	eat(out),
	taste(neutral),
	isVegan(notveg),
	hunger(medium),
	sauceBase(balsamic).

salad(beet):-
	cuisine(slavonian),
	budget(three),
	eat(out),
	taste(spicy),
	isVegan(veg),
	hunger(low),
	sauceBase(noMatter).

salad(beet):-
	cuisine(slavonian),
	budget(three),
	eat(out),
	taste(spicy),
	isVegan(notveg),
	hunger(low),
	sauceBase(noMatter).

salad(omar):-
	cuisine(european),
	budget(five),
	eat(in),
	taste(neutral),
	isVegan(veg),
	hunger(high),
	sauceBase(noMatter).

salad(omar):-
	cuisine(european),
	budget(five),
	eat(in),
	taste(neutral),
	isVegan(notveg),
	hunger(high),
	sauceBase(noMatter).

salad(valdorf):-
	cuisine(european),
	budget(four),
	eat(in),
	taste(sweet),
	isVegan(notveg),
	hunger(high),
	sauceBase(mayonnaise).

salad(crabLui):-
	cuisine(european),
	budget(five),
	eat(in),
	taste(sour),
	isVegan(veg),
	hunger(medium),
	sauceBase(cream).

salad(crabLui):-
	cuisine(european),
	budget(five),
	eat(in),
	taste(sour),
	isVegan(notveg),
	hunger(medium),
	sauceBase(cream).

salad(cherchil):-
	cuisine(european),
	budget(three),
	eat(in),
	taste(sweet),
	isVegan(veg),
	hunger(medium),
	sauceBase(balsamic).

salad(cherchil):-
	cuisine(european),
	budget(three),
	eat(in),
	taste(sweet),
	isVegan(notveg),
	hunger(medium),
	sauceBase(balsamic).

salad(cobb):-
	cuisine(european),
	budget(five),
	eat(in),
	taste(neutral),
	isVegan(notveg),
	hunger(hidh),
	sauceBase(balsamic).

salad(hotTuna):-
	cuisine(european),
	budget(three),
	eat(in),
	taste(neutral),
	isVegan(veg),
	hunger(medium),
	sauceBase(sunflower).

salad(hotTuna):-
	cuisine(european),
	budget(three),
	eat(in),
	taste(neutral),
	isVegan(notveg),
	hunger(medium),
	sauceBase(sunflower).

salad(octopus):-
	cuisine(european),
	budget(five),
	eat(out),
	taste(sour),
	isVegan(veg),
	hunger(medium),
	sauceBase(lemon).

salad(octopus):-
	cuisine(european),
	budget(five),
	eat(out),
	taste(sour),
	isVegan(notveg),
	hunger(medium),
	sauceBase(lemon).

salad(quesadilla):-
	cuisine(european),
	budget(three),
	eat(out),
	taste(neutral),
	isVegan(veg),
	hunger(low),
	sauceBase(cream).

salad(quesadilla):-
	cuisine(european),
	budget(three),
	eat(out),
	taste(neutral),
	isVegan(notveg),
	hunger(low),
	sauceBase(cream).

salad(brieDuck):-
	cuisine(european),
	budget(four),
	eat(out),
	taste(sweet),
	isVegan(notveg),
	hunger(high),
	sauceBase(noMatter).

salad(seafoodVeg):-
	cuisine(european),
	budget(five),
	eat(out),
	taste(neutral),
	isVegan(veg),
	hunger(high),
	sauceBase(noMatter).

salad(seafoodVeg):-
	cuisine(european),
	budget(five),
	eat(out),
	taste(neutral),
	isVegan(notveg),
	hunger(high),
	sauceBase(noMatter).

salad(kress):-
	cuisine(slavonian),
	budget(one),
	eat(in),
	taste(neutral),
	isVegan(veg),
	hunger(low),
	sauceBase(sunflower).

salad(kress):-
	cuisine(slavonian),
	budget(one),
	eat(in),
	taste(neutral),
	isVegan(notveg),
	hunger(low),
	sauceBase(sunflower).

salad(default):-
	cuisine(_),
	budget(_),
	eat(_),
	taste(_),
	isVegan(_),
	hunger(_).
