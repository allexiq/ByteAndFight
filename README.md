Daca la rularea codului exista erori, se va rula DOAR Source.cpp, ce conține codul din toate celelalte fișiere .h și .cpp

Byte & Fight – descriere
Jocul în cauză se bazează pe un sistem de luptă în timp real, în care jucătorul controlează un personaj principal (player-ul) și interacționează cu o serie de inamici, fiecare cu propriile sale abilități și comportamente. Personajul principal se află într-o lume virtuală, în care trebuie să depășească obstacole, să înfrunte inamici și să adune puncte de progres. În plus, jucătorul poate alege dificultatea jocului, ceea ce influențează atât forța inamicilor, cât și dinamica generală a jocului.
1. Player-ul (personajul principal)
Personajul principal este controlat de jucător și poate realiza mai multe acțiuni, precum mișcarea, saltul, atacul și evitarea atacurilor inamicilor. În funcție de starea jocului, personajul va trece prin diferite etape de animație, reflectând starea sa actuală. De exemplu, când nu face nimic, va fi în stare Idle (odihnă), iar când se mișcă, va intra în starea Run. Când se află în aer, va fi în starea Jump, iar în momentul în care atacă, va comuta la animația Attack.
Personajul principal poate suferi daune din partea inamicilor, dar are o bară de viață care scade atunci când este lovit. Dacă viața ajunge la zero, personajul va muri și va trebui să fie resetat, iar jucătorul va trebui să înceapă din nou.
2. Zombie-ul
Zombie-ul este un inamic clasic în multe jocuri de acțiune. În acest joc, el are comportamente specifice, cum ar fi mișcarea pe orizontală în căutarea player-ului și atacarea acestuia atunci când se află la o distanță suficient de mică. Zombie-ul are o viață mică și moare instantaneu după ce primește un hit. De asemenea, el are animații proprii care reflectă stările sale, cum ar fi Idle, Walk, Attack și Dead.
Atunci când zombie-ul este lovit, va intra în starea Dead și va reda animația corespunzătoare, indicând faptul că acesta a fost învins. De asemenea, zombie-ul poate fi invulnerabil pentru o scurtă perioadă de timp, după ce a suferit un atac, ceea ce adaugă o dinamică interesantă jocului, evitând ca jucătorul să-l lovească în mod repetat fără să sufere daune.
3. Boss-ul
Boss-ul este un inamic mult mai puternic decât zombie-ul și se află într-o stare constantă de activitate. Acesta poate efectua o varietate de atacuri, are o viață mult mai mare și este capabil să se miște pe verticală și orizontală. Boss-ul poate utiliza o serie de animații pentru a reflecta stările sale, inclusiv Idle, Walk, Attack, Hurt, și Dead. Animațiile acestuia sunt mai complexe și mai detaliate, iar atacurile sale sunt mult mai puternice și mai diversificate.
Când boss-ul este învins, trece în starea Dead, iar animația sa reflectă acest lucru, cu o schimbare a comportamentului și a aspectului.
Jocul include un sistem de dificultate care permite jucătorilor să aleagă cum vor să se joace. La început, jucătorul poate selecta una dintre opțiunile de dificultate: Ușor, Medie sau Greu. Fiecare nivel de dificultate influențează comportamentele inamicilor, viteza lor, forța atacurilor și capacitatea de a se recupera după un atac. De exemplu, pe nivelul Greu, zombie-urile și boss-ul sunt mai rapide, mai agresive și au mai multă viață, în timp ce pe nivelul Ușor, aceștia sunt mai lent și suferă daune mai mari.
Combatul se bazează pe o serie de atacuri pe care jucătorul le poate efectua pentru a înfrunta inamicii. Atacurile sunt realizate prin animații care schimbă aspectul și comportamentul personajului principal. Jucătorul trebuie să învețe când să atace, când să evite și când să apere pentru a supraviețui. De asemenea, jucătorul trebuie să își gestioneze viața și resursele, deoarece un singur hit poate fi fatal într-o luptă cu un boss sau cu un grup de zombie.
Jocul contine:
	-fisiere separate pentru clase (.h si .cpp)
	-nu contine using namespace std
	-clase derivate din clasa de baza Entity: Player, Zombie, Boss
	-functii virtuale
	-Zombie este o clasa cu atribute de tip pointer
	-functii 
	-STL
