# Google Test

Biblioteka *googletest*  nalazi se na *GitHub* poslužitelju, na adresi 
[github.com/google/googletest](https://github.com/google/googletest). 
Dokumentacija se može konzultirati na stranici [google.github.io/googletest](https://google.github.io/googletest).
Preuzimanje biblioteke se radi *Git* naredbom:

```
git clone https://github.com/google/googletest.git
```

## Kompilacija googletest biblioteke
 
U direktoriju u kojem smo izvršili ovu naredbu Git će kreirati  direktorij `googletest` i smjestiti 
sav kod biblioteke u njega. Kompilaciju vršimo pomoću *CMake* sustava. Prvo kreiramo direktorij u kojem
će biti izvršni kod, odnosno biblioteke s kojima ćemo povezivati programe koji koriste `googletest`.
U ovom primjeru kreirat ćemo direktorij naziva `build-googletest` na istoj razini s direktorijem 
`googletest`. Sada trebamo ući u direktorij  `build-googletest` i izvršiti sljedeće naredbe:

```
cmake ../googletest/
cmake --build .
```

## Uključivanje googletest biblioteke u CMake projekt


U [dokumentaciji](https://google.github.io/googletest/quickstart-cmake.html) googletest-a se proporuča 
uključivanje izvornog koda googletest biblioteke u vlastiti projekt. Prednost toga je u tome što će
projekt i googletest biti kompilirani i linkani s istim opcijama i bibliotekama. 

Za preuzimanje i kompilaciju googletesta koristimo novi CMake modul 
[FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) koji je prisutan u 
CMake-u od verzije 3.11. 
U korijensku `CMakeLists.txt`  datoteku trebamo uključiti sljedeći sadržaj:

```cmake
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG     e2239ee6043f73722e7aa812a459f54a28552929  # release-1.11.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
```

`GIT_TAG` treba uvijek biti najnoviji.  Prilikom kompilacije googletest će biti preuzet u 
`_dep` poddirektoriju direktorija izvornog koda i tamo će biti i izrađen. 

## Izrada testova

Testove ćemo izrađivati u `test` poddirektoriju, premda ime direktorija može biti proizvoljno.
Jedan primjer `CMakeLists.txt` datoteke u `test` direktoriju može biti sljedeći:

```
add_executable(utest utest.cpp ../src/vector.h)
target_link_libraries(utest  gtest_main)

add_test(NAME utest COMMAND utest)
```

Program s testovima je `utest.cpp`, a izvršni program će se zvati `utest`. Izvršni program uvijek 
ovisi i o kodu koji testira, što je u ovom slučaju samo datoteka `../src/vector.h`. Izvršni program 
povezujemo s bibliotekom `gtest_main` (a ne s  `gtest`) jer  `gtest_main` daje i `main()` funkciju 
te stoga u izvornom kodu  `utest.cpp` pišemo samo testove, a biblioteka osigurava  `main()` funkciju.

Naredba 
[add_test()](https://cmake.org/cmake/help/latest/command/add_test.html) omogućava da naše testove
izvršimo pomoću naredbe *ctest* koja je dio CMake sustava. 

## Pisanje testova

Ovdje je dan jedan primjer datoteke s testovima. 


```c++
#include "gtest/gtest.h"
#include "../src/vector.h"

TEST(Vector, konstruktor){
   Vec<double> a(3);
   EXPECT_EQ(a.size(), 3); 

   Vec<double> b;
   EXPECT_EQ(b.size(), 0); 

   Vec<double> c(5);
   for(auto & x : c) 
       x = 1.0;
   EXPECT_EQ(c[4], 1.0);
}  
```

- Moramo uključiti zaglavlje `gtest/gtest.h`.
- Testovi se pišu unutar `TEST()` makroa koji ekspandira u jednu funkciju tipa `void`. 
- Uvjeti za koje očekujemo da su ispunjeni provjeravaju se pomoću makroa kao što su `EXPECT_EQ()` 
  ili `ASSERT_EQ()`.

## TEST() makro

Makro `TEST()` se koristi _kao_ funkcija dva argumenta: prvi argument je ime kolekcije testova, a drugi 
argument je ime testa. Na taj način grupiramo slične testove u kolekcije testova. Oba argumenta moraju biti
regularna C{pp} imena _bez podvlake_. U našem primjeru kolekcija testova je nazvana `Vector`, a jedini test 
koji imamo je nazvan `konstruktor`. 

Makroi za testiranje uvjeta dolaze u dvije forme: kao  `ASSERT_*` i kao  `EXPECT_*`. Prva verzija u slučaju 
nezadovoljenog uvjeta generira grešku i abortira test u kojem se nalazi. Druga verzija ne abortira test već
samo izvještava o grešci. 

Kako googletest ne daje nikave informacije o razlogu greške programer može dodati svoj komentar 
koji će biti ispisan u slučaju greške. Za to tretiramo  `ASSERT_*`/`EXPECT_*` kao _stream_ i šaljemo 
poruku na njega. Na primjer:


```c++
TEST(Vector, konstruktor){
   //...
   Vec<double> b;
   EXPECT_EQ(b.size(), 1) << "Šalim se!";
   //...
}
```

## Ispitivanje uvjeta (1)

Za ispitivanje uvjeta postoji čitav niz makroa. 
Za detalje pogledajte [Assertions Reference](https://google.github.io/googletest/reference/assertions.html).

Svaki makro ispituje svoj uvjet i javlja grešku ako uvjet nije ispunjen. Pored  `EXPECT_*` makroa postoji
uvijek i `ASSERT_*` makro koji obustavlja test u slučaju greške, dok  `EXPECT_*` nastavlja testiranje. Inače 
imaju istu funkcionalnost.


- `EXPECT_TRUE(uvjet)`; `EXPECT_FALSE(uvjet)`; ispituje da li je uvjet istinit ili nije. 
- `EXPECT_EQ(val1,val2)`; Ispitije `val1 == val2`.
- `EXPECT_NE(val1,val2)`; Ispitije `val1 != val2`.
- `EXPECT_LT(val1,val2)`; Ispitije `val1 < val2`.
- `EXPECT_LE(val1,val2)`; Ispitije `val1 <= val2`.
- `EXPECT_GT(val1,val2)`; Ispitije `val1 > val2`.
- `EXPECT_GE(val1,val2)`; Ispitije `val1 >= val2`.

Na pokazivačima ovi makroi uspoređuju same pokazivače.

## Ispitivanje uvjeta (2) C-stringovi

Za ispitivanje jednakosti/nejednakosti `std::string` objekata koristimo  `EXPECT_EQ`/`EXPECT_NE`.
Za uspoređivanje C-stringova trebaju na posebni makroi: 

- `EXPECT_STREQ(str1,str2)`;  ispituje jednakost stringova.
- `EXPECT_STRNE(str1,str2)`;  ispituje nejednakost stringova.
- `EXPECT_STRCASEEQ(str1,str2)`;  ispituje jednakost stringova neovisno o velikim/malim slovima.
- `EXPECT_STRCASENE(str1,str2)`;  ispituje nejednakost stringova neovisno o velikim/malim slovima.


## Ispitivanje uvjeta (3) realni brojevi

Brojevi s pokretnim zarezom redovito sadrže određenu grešku zaokruživanja te stoga njihovu jednakost 
ne uspoređujemo egzaktno. Sljedeća tri makroa omogućavaju uspoređivanje na osnovu relativne ili apsolute 
razlike između brojeva:

- `EXPECT_FLOAT_EQ(val1,val2)` provjeri da su dvije `float` vrijednosti jednake do na četiri ULP-a.
- `EXPECT_DOUBLE_EQ(val1,val2)`  provjeri da su dvije `double` vrijednosti jednake do na četiri ULP-a.
- `EXPECT_NEAR(val1,val2,abs_error)`  provjeri da razlika `val1` i `val2` ne prelazi `abs_error`.

## Ispitivanje uvjeta (4) izuzeci

Možemo ispitivati hoće li dio koda izbaciti izuzetak ili neće. 



- `EXPECT_THROW(naredba,exception_type)`; Ispituje hoće li naredba izbaciti izuzetak danog tipa.
- `EXPECT_ANY_THROW(naredba)`; Ispituje hoće li naredba izbaciti izuzetak (bilo koji).
- `EXPECT_NO_THROW(naredba)`; Ispituje hoće li naredba ne izbaciti izuzetak.


##  Ispitivanje uvjeta (5) predikati

Pomoću makroa `EXPECT_PREDn` možemo ispitivati da li određeni predikat vraća `true` (što se očekuje)
ili `false` (što je greška). `n` u imenu testa je broj argumenata predikata i može ići od 1 do 5. 
Na primjer,

```c++
// testiranje predikata
bool isCoprime(long a, long b) { return gcd(a,b) == 1; }

TEST(predTest, basic){
    EXPECT_PRED2(isCoprime, 3, 5); // o.k.
    long a = 6, b = 12;
    EXPECT_PRED2(isCoprime, a, b); // greška
}
```

`EXPECT_PRED2` će formatirati sljedeću poruku:

```
isCoprime(a, b) evaluates to false, where
1: a evaluates to 6
1: b evaluates to 12
1: [  FAILED  ] predTest.basic (0 ms)
```

Vidjeti [googletest dokumentaciju](https://google.github.io/googletest/reference/assertions.html#EXPECT_PRED)
za detalje vezane za predloške i propterećene predikate.

##  Ispitivanje uvjeta (6) predikati

Ako želimo bolju poruku o grešci pri korištenja predikata možemo predikat konstruirati tako 
da vrati (google test) tip `testing::AssertionResult`. Umjesto `true` vraća `testing::AssertionSuccess()`,
a umjesto  `false`, `testing::AssertionFailure()`. Te dvije vrijednosti djeluju kao streamovi na koje možemo
poslati našu poruku. Na primjer:


```c++
testing::AssertionResult isCoprime1(long a, long b) {
        if(gcd(a,b) == 1)
            return testing::AssertionSuccess();
        else
            return testing::AssertionFailure();
}

TEST(predTest, advanced){
    EXPECT_PRED2(isCoprime1, 3, 5) << " o.k." << 3 << " i " << 5 << " su relativno prosti";
    long a = 6, b = 12;
    EXPECT_PRED2(isCoprime1, a, b) << a << " i " << b << " nisu relativno prosti"; 
}
```

Poruka o grešci:

```
isCoprime1(a, b) evaluates to false, where
1: a evaluates to 6
1: b evaluates to 12
1: 6 i 12 nisu relativno prosti
1: [  FAILED  ] predTest.advanced (0 ms)
```


