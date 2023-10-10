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
    EXPECT_PRED2(isCoprime1, 3, 5);
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


Poruka koju šaljemo u `AssertionResult` tok javlja se samo ako uvjet nije zadovoljen.

## Test Fixtures 

Testovi često trebaju određenu inicijalizaciju prije svog izvršavanja te destrukciju/dealokaciju 
objekata koji su pri tome konstruirani/alocirani, nakon što završe. Ukoliko se ista alokacija/dealokacija
ponavlja u više testova možemo tu funkcionalnost zatvoriti u jednu klasu koja se naziva _test fixture_.

Ta klasa treba imati sljedeći oblik (ime klase je proizvoljno):


```c++
class myTestFixture1: public ::testing::Test { 
protected: 
   myTestFixture1( ) { 
       // Konstruktor.
   } 
   void SetUp( ) { 
       // Ovaj kod će se izvršiti prije testova.
   }
   void TearDown( ) { 
       // Kod koji se izvršava nakon testova. Dozvoljava izuzetke.
   }
   ~myTestFixture1( )  { 
       // Destruktor. Izuzeci vode na nedefinirano ponašanje.   
   }
   // Potrebne varijable i metode.
};
```


- Klasa nasljeđuje klasu `testing::Test` i cijelo tijelo klase je u `protected` sekciji. 
- Klasa treba imati dodijeljeni konstruktir ili `SetUp()` metodu te destruktor ili `TearDown()` metodu. 
- Izbor konstruktor/`SetUp()`, destruktor/`TearDown()` je stvar slobodnog odabira - `TearDown()` za razliku od 
  destruktora dozvoljava izuzetke.
- Unutar klase dolaze varijable i metode koje želimo koristiti u različitim testovima. 

IMPORTANT: Novi _fixture_ objekt se konstruira u svakom novom testu. Na taj način testovi ostaju 
nezavisni premda koriste istu _fixture_ klasu.

Testovi koji koriste _fixture_ klase moraju koristiti makro `TEST_F()` umjesto makroa `TEST()`
i prvi argument  `TEST_F()` makroa mora biti ime _fixture_ klase.

```c++
TEST_F(myTestFixture1, Test1){
   // ovdje dolaze testovi. Konstrukcija i destrukcija 
   // myTestFixture1 objekta je automatska
}
```


### Kod za testiranje

Testirat ćemo  klasu `Vector` koja predstavlja (ne baš standardan) vektor (vidjeti datoteku `src/mylib.h`). 
Za testiranje parametriziranog konstruktora kopije zgodno je imati već 
konstruirane vektore i stoga koristimo _test fixture_ klasu.


###  Test Fixtures i testovi           

Pripadna _test fixture_ klasa definira i inicijalizira dva vektora različite 
duljine koje možemo koristiti u testovima.

```c++
class VectorTest : public ::testing::Test{ // Fixture
protected:
    Vector<int, 3> vi3;
    Vector<double, 4> vd4;
    VectorTest(){
       vi3[0] = 0; vi3[1]=1; vi3[2] = 2; 
       vd4[0] = 0.0; vd4[1] = 1.0; vd4[2] = 2.0; vd4[3] = 3.0;
    }
};
```

U testovima provjeravamo konstrukciju kopiranjem pomoću duljeg i kraćeg vektora.

```c++
TEST_F(VectorTest, ConstructFromSmaller){
    Vector<double, 5> v(vd4);
    EXPECT_DOUBLE_EQ(v[0], vd4[0]);
    EXPECT_DOUBLE_EQ(v[3], 3.0);
    EXPECT_DOUBLE_EQ(v[4], 0.0);
}
TEST_F(VectorTest, ConstructFromGreater){
    Vector<double, 2> v(vi3);
    EXPECT_DOUBLE_EQ(v[0], vi3[0]);
    EXPECT_DOUBLE_EQ(v[1], vi3[1]);
}
```

## Parametrizirani testovi

Dvije su vrste parametriziranih testova, ovisno o tome čime su parametrizirani.
Parametri mogu biti *vrijednosti* ili *tipovi*.

Kada su parametri vrijednosti, onda isti kod možemo testirati s različitim 
vrijednostima parametara bez ponavljanja testova.
U slučaju parametara koji su tipovi, predloške klasa i funkcija možemo testirati 
s različitim parametrima predloška bez ponavljanja testova.


### Vrijednosti kao parametri

Potrebno je napraviti _test fixture_ klasu, no sada ju izvodimo iz `testing::TestWithParam<T>` klase 
u kojoj je `T` tip parametra koji variramo:


```c++
class MyTest : public testing::TestWithParam<T> {
  // Uobičajena fixture klasa. Test parametar dohvaćamo pomoću 
  // GetParam() metode iz bazne klase.
};
```

Za testove koristimo makro `TEST_P()` čiji je prvi parametar ime _test fixture_ klase:

```c++
TEST_P(MyTest, ImeTesta) {
  //...
}
```

Unutar testova parametar za koji se vrši test dobivamo pomoću *GetParam()* funkcije. 

Zadnji korak je selekcija parametara koja se radi pomoću makro 
[INSTANTIATE_TEST_SUITE_P](https://google.github.io/googletest/reference/testing.html#INSTANTIATE_TEST_SUITE_P),
koji ima signaturu:

```
INSTANTIATE_TEST_SUITE_P(InstantiationName,TestSuiteName,param_generator)
```

Ovdje je `InstantiationName` ime koje se koristi za ovaj niz testova da se razlikuje od drugih nizova testova.
`TestSuiteName` je ime _fixture_ klase, a `param_generator` je generator vrijednosti.



|  Generator vrijednosti |	Ponašanje |
| -----------------------| ---------  |
| `Range(begin, end [, step])`  | Daje poluotvoreni interval `{begin, begin+step, begin+step+step, ...}`                                   (`step=1` ako nije zadan). |
| `Values(v1, v2, ..., vN)` |	Daje vrijednosti `{v1, v2, ..., vN}`.|
| `ValuesIn(container)` |	Daje vrijednosti C-niza ili STL spremnika `container`.|
| `ValuesIn(begin,end)` |	Daje vrijednosti iz raspona `[begin, end)`.|
| `Bool()` |	Daje niz `{false, true}`.|
| `Combine(g1, g2, ..., gN)` |	Daje Kartezijev produkt vrijednosti koje daju generatori `g1, g2, ... , gN`.|


### Primjer

Testiramo funkciju `strip()` s nizom argumenata:


```c++
class StripTest : public testing::TestWithParam<const char*>{ // fixture
};


TEST_P(StripTest, many){
	const char * res = strip(GetParam());
	auto N = std::strlen(res);
	EXPECT_FALSE( std::isspace(static_cast<unsigned char>(res[0])) );
	EXPECT_FALSE( std::isspace(static_cast<unsigned char>(res[N-1])) );
}

INSTANTIATE_TEST_SUITE_P(StripSkupUlaza,
                         StripTest,
                         testing::Values("  naprijed", "nazad    ", " oba "));
```

Izlaz:

```
1: [----------] 3 tests from StripSkupUlaza/StripTest
1: [ RUN      ] StripSkupUlaza/StripTest.many/0
1: [       OK ] StripSkupUlaza/StripTest.many/0 (0 ms)
1: [ RUN      ] StripSkupUlaza/StripTest.many/1
1: [       OK ] StripSkupUlaza/StripTest.many/1 (0 ms)
1: [ RUN      ] StripSkupUlaza/StripTest.many/2
1: [       OK ] StripSkupUlaza/StripTest.many/2 (0 ms)
1: [----------] 3 tests from StripSkupUlaza/StripTest (0 ms total)
```

### Tipovi kao parametri

Kada kod želimo testirati za različite tipove
prvo definiramo _fixture klasu_ kao *predložak klase* i izvodimo ju iz `testing::Test` klase:


```c++
template <typename T>
class MyTest : public testing::Test {
 public:
  // ...
};
```

Tipove za koje želimo izvršiti testiranje zadajemo pomoću 
[TYPED_TEST_SUITE()](https://google.github.io/googletest/reference/testing.html#TYPED_TEST_SUITE) makroa. Pri tome se niz tipova zadaje pomoću predloška `::testing::Types<>` koji uzima proizvojan
broj parametara predloška.  Na primjer, ako želimo kod testirati na tipovima 
`char`, `int` i  `unsigned int` koristimo sljedeću konstrukciju:


```c++
using MyTypes = ::testing::Types<char, int, unsigned int>;
TYPED_TEST_SUITE(MyTest, MyTypes);
```

Sami testovi se pišu pomoću makroa `TYPED_TEST`:



```c++
TYPED_TEST(MyTest, ImeTesta){
//...
}
```

Test će biti ponovljen za svaku vrijednost parametra. Unutar testa trenutni parametar predloška za 
koji je test generiran može se dohvatiti kroz *TypeParam* simbol.

### Primjer

Uzmimo da je naš vektor parametriziran i s tipom komponente te da dodijeljeni konstruktor
vrši inicijalizaciju nulom.  U _fixture_ klasi 
ćemo definirati jedan vektor `vec`:


```c++
template <typename T>
class ParVectorTest : public ::testing::Test{
public:
    Vector<T, 3> vec;
    ParVectorTest(){
      for(std::size_t i=0; i<3; ++i) vec[i] = 0;
    }
};
```

Kao primjer testirat ćemo parametrizirani konstruktor kopije:


```c++
using MyTypes = ::testing::Types<int, float, double, long, char>;
TYPED_TEST_SUITE(ParVectorTest, MyTypes);

TYPED_TEST(ParVectorTest, equal){
  Vector<TypeParam,4> vecd(this->vec);
  EXPECT_EQ(vecd[0], this->vec[0]);
  EXPECT_EQ(vecd[1], this->vec[1]);
}
```

Izlaz:

```
1: [----------] 1 test from ParVectorTest/0, where TypeParam = int
1: [ RUN      ] ParVectorTest/0.equal
1: [       OK ] ParVectorTest/0.equal (0 ms)
1: [----------] 1 test from ParVectorTest/0 (0 ms total)
1: 
1: [----------] 1 test from ParVectorTest/1, where TypeParam = float
1: [ RUN      ] ParVectorTest/1.equal
1: [       OK ] ParVectorTest/1.equal (0 ms)
1: [----------] 1 test from ParVectorTest/1 (0 ms total)
1: 
1: [----------] 1 test from ParVectorTest/2, where TypeParam = double
1: [ RUN      ] ParVectorTest/2.equal
1: [       OK ] ParVectorTest/2.equal (0 ms)
1: [----------] 1 test from ParVectorTest/2 (0 ms total)
1: 
1: [----------] 1 test from ParVectorTest/3, where TypeParam = long
1: [ RUN      ] ParVectorTest/3.equal
1: [       OK ] ParVectorTest/3.equal (0 ms)
1: [----------] 1 test from ParVectorTest/3 (0 ms total)
1: 
1: [----------] 1 test from ParVectorTest/4, where TypeParam = char
1: [ RUN      ] ParVectorTest/4.equal
1: [       OK ] ParVectorTest/4.equal (0 ms)
1: [----------] 1 test from ParVectorTest/4 (0 ms total)
```

