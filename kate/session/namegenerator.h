// shamelessly stolen from https://raw.githubusercontent.com/moby/moby/master/pkg/namesgenerator/names-generator.go

#include <QStringList>

static const QStringList left_names = {
    QLatin1String("admiring"),  QLatin1String("adoring"),       QLatin1String("affectionate"), QLatin1String("agitated"),      QLatin1String("amazing"),
    QLatin1String("angry"),     QLatin1String("awesome"),       QLatin1String("beautiful"),    QLatin1String("blissful"),      QLatin1String("bold"),
    QLatin1String("boring"),    QLatin1String("brave"),         QLatin1String("busy"),         QLatin1String("charming"),      QLatin1String("clever"),
    QLatin1String("cool"),      QLatin1String("compassionate"), QLatin1String("competent"),    QLatin1String("condescending"), QLatin1String("confident"),
    QLatin1String("cranky"),    QLatin1String("crazy"),         QLatin1String("dazzling"),     QLatin1String("determined"),    QLatin1String("distracted"),
    QLatin1String("dreamy"),    QLatin1String("eager"),         QLatin1String("ecstatic"),     QLatin1String("elastic"),       QLatin1String("elated"),
    QLatin1String("elegant"),   QLatin1String("eloquent"),      QLatin1String("epic"),         QLatin1String("exciting"),      QLatin1String("fervent"),
    QLatin1String("festive"),   QLatin1String("flamboyant"),    QLatin1String("focused"),      QLatin1String("friendly"),      QLatin1String("frosty"),
    QLatin1String("funny"),     QLatin1String("gallant"),       QLatin1String("gifted"),       QLatin1String("goofy"),         QLatin1String("gracious"),
    QLatin1String("great"),     QLatin1String("happy"),         QLatin1String("hardcore"),     QLatin1String("heuristic"),     QLatin1String("hopeful"),
    QLatin1String("hungry"),    QLatin1String("infallible"),    QLatin1String("inspiring"),    QLatin1String("interesting"),   QLatin1String("intelligent"),
    QLatin1String("jolly"),     QLatin1String("jovial"),        QLatin1String("keen"),         QLatin1String("kind"),          QLatin1String("laughing"),
    QLatin1String("loving"),    QLatin1String("lucid"),         QLatin1String("magical"),      QLatin1String("mystifying"),    QLatin1String("modest"),
    QLatin1String("musing"),    QLatin1String("naughty"),       QLatin1String("nervous"),      QLatin1String("nice"),          QLatin1String("nifty"),
    QLatin1String("nostalgic"), QLatin1String("objective"),     QLatin1String("optimistic"),   QLatin1String("peaceful"),      QLatin1String("pedantic"),
    QLatin1String("pensive"),   QLatin1String("practical"),     QLatin1String("priceless"),    QLatin1String("quirky"),        QLatin1String("quizzical"),
    QLatin1String("recursing"), QLatin1String("relaxed"),       QLatin1String("reverent"),     QLatin1String("romantic"),      QLatin1String("sad"),
    QLatin1String("serene"),    QLatin1String("sharp"),         QLatin1String("silly"),        QLatin1String("sleepy"),        QLatin1String("stoic"),
    QLatin1String("strange"),   QLatin1String("stupefied"),     QLatin1String("suspicious"),   QLatin1String("sweet"),         QLatin1String("tender"),
    QLatin1String("thirsty"),   QLatin1String("trusting"),      QLatin1String("unruffled"),    QLatin1String("upbeat"),        QLatin1String("vibrant"),
    QLatin1String("vigilant"),  QLatin1String("vigorous"),      QLatin1String("wizardly"),     QLatin1String("wonderful"),     QLatin1String("xenodochial"),
    QLatin1String("youthful"),  QLatin1String("zealous"),       QLatin1String("zen"),
};

static const QStringList right_names = {
    // Maria Gaetana Agnesi - Italian mathematician, philosopher, theologian and humanitarian. She was the first woman to write a mathematics handbook and the
    // first woman appointed as a Mathematics Professor at a University. https://en.wikipedia.org/wiki/Maria_Gaetana_Agnesi
    QLatin1String("agnesi"),

    // Muhammad ibn Jābir al-Ḥarrānī al-Battānī was a founding father of astronomy.
    // https://en.wikipedia.org/wiki/Mu%E1%B8%A5ammad_ibn_J%C4%81bir_al-%E1%B8%A4arr%C4%81n%C4%AB_al-Batt%C4%81n%C4%AB
    QLatin1String("albattani"),

    // Frances E. Allen, became the first female IBM Fellow in 1989. In 2006, she became the first female recipient of the ACM's Turing Award.
    // https://en.wikipedia.org/wiki/Frances_E._Allen
    QLatin1String("allen"),

    // June Almeida - Scottish virologist who took the first pictures of the rubella virus - https://en.wikipedia.org/wiki/June_Almeida
    QLatin1String("almeida"),

    // Kathleen Antonelli, American computer programmer and one of the six original programmers of the ENIAC - https://en.wikipedia.org/wiki/Kathleen_Antonelli
    QLatin1String("antonelli"),

    // Archimedes was a physicist, engineer and mathematician who invented too many things to list them here. https://en.wikipedia.org/wiki/Archimedes
    QLatin1String("archimedes"),

    // Maria Ardinghelli - Italian translator, mathematician and physicist - https://en.wikipedia.org/wiki/Maria_Ardinghelli
    QLatin1String("ardinghelli"),

    // Aryabhata - Ancient Indian mathematician-astronomer during 476-550 CE https://en.wikipedia.org/wiki/Aryabhata
    QLatin1String("aryabhata"),

    // Wanda Austin - Wanda Austin is the President and CEO of The Aerospace Corporation, a leading architect for the US security space programs.
    // https://en.wikipedia.org/wiki/Wanda_Austin
    QLatin1String("austin"),

    // Charles Babbage invented the concept of a programmable computer. https://en.wikipedia.org/wiki/Charles_Babbage.
    QLatin1String("babbage"),

    // Stefan Banach - Polish mathematician, was one of the founders of modern functional analysis. https://en.wikipedia.org/wiki/Stefan_Banach
    QLatin1String("banach"),

    // Buckaroo Banzai and his mentor Dr. Hikita perfected the QLatin1String("oscillation overthruster"), a device that allows one to pass through solid matter.
    // - https://en.wikipedia.org/wiki/The_Adventures_of_Buckaroo_Banzai_Across_the_8th_Dimension
    QLatin1String("banzai"),

    // John Bardeen co-invented the transistor - https://en.wikipedia.org/wiki/John_Bardeen
    QLatin1String("bardeen"),

    // Jean Bartik, born Betty Jean Jennings, was one of the original programmers for the ENIAC computer. https://en.wikipedia.org/wiki/Jean_Bartik
    QLatin1String("bartik"),

    // Laura Bassi, the world's first female professor https://en.wikipedia.org/wiki/Laura_Bassi
    QLatin1String("bassi"),

    // Hugh Beaver, British engineer, founder of the Guinness Book of World Records https://en.wikipedia.org/wiki/Hugh_Beaver
    QLatin1String("beaver"),

    // Alexander Graham Bell - an eminent Scottish-born scientist, inventor, engineer and innovator who is credited with inventing the first practical telephone
    // - https://en.wikipedia.org/wiki/Alexander_Graham_Bell
    QLatin1String("bell"),

    // Karl Friedrich Benz - a German automobile engineer. Inventor of the first practical motorcar. https://en.wikipedia.org/wiki/Karl_Benz
    QLatin1String("benz"),

    // Homi J Bhabha - was an Indian nuclear physicist, founding director, and professor of physics at the Tata Institute of Fundamental Research. Colloquially
    // known as QLatin1String("father of Indian nuclear programme"- https://en.wikipedia.org/wiki/Homi_J._Bhabha
    QLatin1String("bhabha"),

    // Bhaskara II - Ancient Indian mathematician-astronomer whose work on calculus predates Newton and Leibniz by over half a millennium -
    // https://en.wikipedia.org/wiki/Bh%C4%81skara_II#Calculus
    QLatin1String("bhaskara"),

    // Sue Black - British computer scientist and campaigner. She has been instrumental in saving Bletchley Park, the site of World War II codebreaking -
    // https://en.wikipedia.org/wiki/Sue_Black_(computer_scientist)
    QLatin1String("black"),

    // Elizabeth Helen Blackburn - Australian-American Nobel laureate; best known for co-discovering telomerase.
    // https://en.wikipedia.org/wiki/Elizabeth_Blackburn
    QLatin1String("blackburn"),

    // Elizabeth Blackwell - American doctor and first American woman to receive a medical degree - https://en.wikipedia.org/wiki/Elizabeth_Blackwell
    QLatin1String("blackwell"),

    // Niels Bohr is the father of quantum theory. https://en.wikipedia.org/wiki/Niels_Bohr.
    QLatin1String("bohr"),

    // Kathleen Booth, she's credited with writing the first assembly language. https://en.wikipedia.org/wiki/Kathleen_Booth
    QLatin1String("booth"),

    // Anita Borg - Anita Borg was the founding director of the Institute for Women and Technology (IWT). https://en.wikipedia.org/wiki/Anita_Borg
    QLatin1String("borg"),

    // Satyendra Nath Bose - He provided the foundation for Bose–Einstein statistics and the theory of the Bose–Einstein condensate. -
    // https://en.wikipedia.org/wiki/Satyendra_Nath_Bose
    QLatin1String("bose"),

    // Katherine Louise Bouman is an imaging scientist and Assistant Professor of Computer Science at the California Institute of Technology. She researches
    // computational methods for imaging, and developed an algorithm that made possible the picture first visualization of a black hole using the Event Horizon
    // Telescope. - https://en.wikipedia.org/wiki/Katie_Bouman
    QLatin1String("bouman"),

    // Evelyn Boyd Granville - She was one of the first African-American woman to receive a Ph.D. in mathematics; she earned it in 1949 from Yale University.
    // https://en.wikipedia.org/wiki/Evelyn_Boyd_Granville
    QLatin1String("boyd"),

    // Brahmagupta - Ancient Indian mathematician during 598-670 CE who gave rules to compute with zero - https://en.wikipedia.org/wiki/Brahmagupta#Zero
    QLatin1String("brahmagupta"),

    // Walter Houser Brattain co-invented the transistor - https://en.wikipedia.org/wiki/Walter_Houser_Brattain
    QLatin1String("brattain"),

    // Emmett Brown invented time travel. https://en.wikipedia.org/wiki/Emmett_Brown (thanks Brian Goff)
    QLatin1String("brown"),

    // Linda Brown Buck - American biologist and Nobel laureate best known for her genetic and molecular analyses of the mechanisms of smell.
    // https://en.wikipedia.org/wiki/Linda_B._Buck
    QLatin1String("buck"),

    // Dame Susan Jocelyn Bell Burnell - Northern Irish astrophysicist who discovered radio pulsars and was the first to analyse them.
    // https://en.wikipedia.org/wiki/Jocelyn_Bell_Burnell
    QLatin1String("burnell"),

    // Annie Jump Cannon - pioneering female astronomer who classified hundreds of thousands of stars and created the system we use to understand stars today.
    // https://en.wikipedia.org/wiki/Annie_Jump_Cannon
    QLatin1String("cannon"),

    // Rachel Carson - American marine biologist and conservationist, her book Silent Spring and other writings are credited with advancing the global
    // environmental movement. https://en.wikipedia.org/wiki/Rachel_Carson
    QLatin1String("carson"),

    // Dame Mary Lucy Cartwright - British mathematician who was one of the first to study what is now known as chaos theory. Also known for Cartwright's
    // theorem which finds applications in signal processing. https://en.wikipedia.org/wiki/Mary_Cartwright
    QLatin1String("cartwright"),

    // George Washington Carver - American agricultural scientist and inventor. He was the most prominent black scientist of the early 20th century.
    // https://en.wikipedia.org/wiki/George_Washington_Carver
    QLatin1String("carver"),

    // Vinton Gray Cerf - American Internet pioneer, recognised as one of QLatin1String("the fathers of the Internet". With Robert Elliot Kahn, he designed TCP
    // and IP, the primary data communication protocols of the Internet and other computer networks. https://en.wikipedia.org/wiki/Vint_Cerf
    QLatin1String("cerf"),

    // Subrahmanyan Chandrasekhar - Astrophysicist known for his mathematical theory on different stages and evolution in structures of the stars. He has won
    // nobel prize for physics - https://en.wikipedia.org/wiki/Subrahmanyan_Chandrasekhar
    QLatin1String("chandrasekhar"),

    // Sergey Alexeyevich Chaplygin (Russian: Серге́й Алексе́евич Чаплы́гин; April 5, 1869 – October 8, 1942) was a Russian and Soviet physicist, mathematician,
    // and mechanical engineer. He is known for mathematical formulas such as Chaplygin's equation and for a hypothetical substance in cosmology called
    // Chaplygin gas, named after him. https://en.wikipedia.org/wiki/Sergey_Chaplygin
    QLatin1String("chaplygin"),

    // Émilie du Châtelet - French natural philosopher, mathematician, physicist, and author during the early 1730s, known for her translation of and commentary
    // on Isaac Newton's book Principia containing basic laws of physics. https://en.wikipedia.org/wiki/%C3%89milie_du_Ch%C3%A2telet
    QLatin1String("chatelet"),

    // Asima Chatterjee was an Indian organic chemist noted for her research on vinca alkaloids, development of drugs for treatment of epilepsy and malaria -
    // https://en.wikipedia.org/wiki/Asima_Chatterjee
    QLatin1String("chatterjee"),

    // David Lee Chaum - American computer scientist and cryptographer. Known for his seminal contributions in the field of anonymous communication.
    // https://en.wikipedia.org/wiki/David_Chaum
    QLatin1String("chaum"),

    // Pafnuty Chebyshev - Russian mathematician. He is known fo his works on probability, statistics, mechanics, analytical geometry and number theory
    // https://en.wikipedia.org/wiki/Pafnuty_Chebyshev
    QLatin1String("chebyshev"),

    // Joan Clarke - Bletchley Park code breaker during the Second World War who pioneered techniques that remained top secret for decades. Also an accomplished
    // numismatist https://en.wikipedia.org/wiki/Joan_Clarke
    QLatin1String("clarke"),

    // Bram Cohen - American computer programmer and author of the BitTorrent peer-to-peer protocol. https://en.wikipedia.org/wiki/Bram_Cohen
    QLatin1String("cohen"),

    // Jane Colden - American botanist widely considered the first female American botanist - https://en.wikipedia.org/wiki/Jane_Colden
    QLatin1String("colden"),

    // Gerty Theresa Cori - American biochemist who became the third woman—and first American woman—to win a Nobel Prize in science, and the first woman to be
    // awarded the Nobel Prize in Physiology or Medicine. Cori was born in Prague. https://en.wikipedia.org/wiki/Gerty_Cori
    QLatin1String("cori"),

    // Seymour Roger Cray was an American electrical engineer and supercomputer architect who designed a series of computers that were the fastest in the world
    // for decades. https://en.wikipedia.org/wiki/Seymour_Cray
    QLatin1String("cray"),

    // This entry reflects a husband and wife team who worked together:
    // Joan Curran was a Welsh scientist who developed radar and invented chaff, a radar countermeasure. https://en.wikipedia.org/wiki/Joan_Curran
    // Samuel Curran was an Irish physicist who worked alongside his wife during WWII and invented the proximity fuse.
    // https://en.wikipedia.org/wiki/Samuel_Curran
    QLatin1String("curran"),

    // Marie Curie discovered radioactivity. https://en.wikipedia.org/wiki/Marie_Curie.
    QLatin1String("curie"),

    // Charles Darwin established the principles of natural evolution. https://en.wikipedia.org/wiki/Charles_Darwin.
    QLatin1String("darwin"),

    // Leonardo Da Vinci invented too many things to list here. https://en.wikipedia.org/wiki/Leonardo_da_Vinci.
    QLatin1String("davinci"),

    // A. K. (Alexander Keewatin) Dewdney, Canadian mathematician, computer scientist, author and filmmaker. Contributor to Scientific American's
    // QLatin1String("Computer Recreations" from 1984 to 1991. Author of Core War (program), The Planiverse, The Armchair Universe, The Magic Machine, The New
    // Turing Omnibus, and more. https://en.wikipedia.org/wiki/Alexander_Dewdney
    QLatin1String("dewdney"),

    // Satish Dhawan - Indian mathematician and aerospace engineer, known for leading the successful and indigenous development of the Indian space programme.
    // https://en.wikipedia.org/wiki/Satish_Dhawan
    QLatin1String("dhawan"),

    // Bailey Whitfield Diffie - American cryptographer and one of the pioneers of public-key cryptography. https://en.wikipedia.org/wiki/Whitfield_Diffie
    QLatin1String("diffie"),

    // Edsger Wybe Dijkstra was a Dutch computer scientist and mathematical scientist. https://en.wikipedia.org/wiki/Edsger_W._Dijkstra.
    QLatin1String("dijkstra"),

    // Paul Adrien Maurice Dirac - English theoretical physicist who made fundamental contributions to the early development of both quantum mechanics and
    // quantum electrodynamics. https://en.wikipedia.org/wiki/Paul_Dirac
    QLatin1String("dirac"),

    // Agnes Meyer Driscoll - American cryptanalyst during World Wars I and II who successfully cryptanalysed a number of Japanese ciphers. She was also the
    // co-developer of one of the cipher machines of the US Navy, the CM. https://en.wikipedia.org/wiki/Agnes_Meyer_Driscoll
    QLatin1String("driscoll"),

    // Donna Dubinsky - played an integral role in the development of personal digital assistants (PDAs) serving as CEO of Palm, Inc. and co-founding
    // Handspring. https://en.wikipedia.org/wiki/Donna_Dubinsky
    QLatin1String("dubinsky"),

    // Annie Easley - She was a leading member of the team which developed software for the Centaur rocket stage and one of the first African-Americans in her
    // field. https://en.wikipedia.org/wiki/Annie_Easley
    QLatin1String("easley"),

    // Thomas Alva Edison, prolific inventor https://en.wikipedia.org/wiki/Thomas_Edison
    QLatin1String("edison"),

    // Albert Einstein invented the general theory of relativity. https://en.wikipedia.org/wiki/Albert_Einstein
    QLatin1String("einstein"),

    // Alexandra Asanovna Elbakyan (Russian: Алекса́ндра Аса́новна Элбакя́н) is a Kazakhstani graduate student, computer programmer, internet pirate in hiding, and
    // the creator of the site Sci-Hub. Nature has listed her in 2016 in the top ten people that mattered in science, and Ars Technica has compared her to Aaron
    // Swartz. - https://en.wikipedia.org/wiki/Alexandra_Elbakyan
    QLatin1String("elbakyan"),

    // Taher A. ElGamal - Egyptian cryptographer best known for the ElGamal discrete log cryptosystem and the ElGamal digital signature scheme.
    // https://en.wikipedia.org/wiki/Taher_Elgamal
    QLatin1String("elgamal"),

    // Gertrude Elion - American biochemist, pharmacologist and the 1988 recipient of the Nobel Prize in Medicine - https://en.wikipedia.org/wiki/Gertrude_Elion
    QLatin1String("elion"),

    // James Henry Ellis - British engineer and cryptographer employed by the GCHQ. Best known for conceiving for the first time, the idea of public-key
    // cryptography. https://en.wikipedia.org/wiki/James_H._Ellis
    QLatin1String("ellis"),

    // Douglas Engelbart gave the mother of all demos: https://en.wikipedia.org/wiki/Douglas_Engelbart
    QLatin1String("engelbart"),

    // Euclid invented geometry. https://en.wikipedia.org/wiki/Euclid
    QLatin1String("euclid"),

    // Leonhard Euler invented large parts of modern mathematics. https://de.wikipedia.org/wiki/Leonhard_Euler
    QLatin1String("euler"),

    // Michael Faraday - British scientist who contributed to the study of electromagnetism and electrochemistry. https://en.wikipedia.org/wiki/Michael_Faraday
    QLatin1String("faraday"),

    // Horst Feistel - German-born American cryptographer who was one of the earliest non-government researchers to study the design and theory of block
    // ciphers. Co-developer of DES and Lucifer. Feistel networks, a symmetric structure used in the construction of block ciphers are named after him.
    // https://en.wikipedia.org/wiki/Horst_Feistel
    QLatin1String("feistel"),

    // Pierre de Fermat pioneered several aspects of modern mathematics. https://en.wikipedia.org/wiki/Pierre_de_Fermat
    QLatin1String("fermat"),

    // Enrico Fermi invented the first nuclear reactor. https://en.wikipedia.org/wiki/Enrico_Fermi.
    QLatin1String("fermi"),

    // Richard Feynman was a key contributor to quantum mechanics and particle physics. https://en.wikipedia.org/wiki/Richard_Feynman
    QLatin1String("feynman"),

    // Benjamin Franklin is famous for his experiments in electricity and the invention of the lightning rod.
    QLatin1String("franklin"),

    // Yuri Alekseyevich Gagarin - Soviet pilot and cosmonaut, best known as the first human to journey into outer space.
    // https://en.wikipedia.org/wiki/Yuri_Gagarin
    QLatin1String("gagarin"),

    // Galileo was a founding father of modern astronomy, and faced politics and obscurantism to establish scientific truth.
    // https://en.wikipedia.org/wiki/Galileo_Galilei
    QLatin1String("galileo"),

    // Évariste Galois - French mathematician whose work laid the foundations of Galois theory and group theory, two major branches of abstract algebra, and the
    // subfield of Galois connections, all while still in his late teens. https://en.wikipedia.org/wiki/%C3%89variste_Galois
    QLatin1String("galois"),

    // Kadambini Ganguly - Indian physician, known for being the first South Asian female physician, trained in western medicine, to graduate in South Asia.
    // https://en.wikipedia.org/wiki/Kadambini_Ganguly
    QLatin1String("ganguly"),

    // William Henry QLatin1String("Bill" Gates III is an American business magnate, philanthropist, investor, computer programmer, and inventor.
    // https://en.wikipedia.org/wiki/Bill_Gates
    QLatin1String("gates"),

    // Johann Carl Friedrich Gauss - German mathematician who made significant contributions to many fields, including number theory, algebra, statistics,
    // analysis, differential geometry, geodesy, geophysics, mechanics, electrostatics, magnetic fields, astronomy, matrix theory, and optics.
    // https://en.wikipedia.org/wiki/Carl_Friedrich_Gauss
    QLatin1String("gauss"),

    // Marie-Sophie Germain - French mathematician, physicist and philosopher. Known for her work on elasticity theory, number theory and philosophy.
    // https://en.wikipedia.org/wiki/Sophie_Germain
    QLatin1String("germain"),

    // Adele Goldberg, was one of the designers and developers of the Smalltalk language. https://en.wikipedia.org/wiki/Adele_Goldberg_(computer_scientist)
    QLatin1String("goldberg"),

    // Adele Goldstine, born Adele Katz, wrote the complete technical description for the first electronic digital computer, ENIAC.
    // https://en.wikipedia.org/wiki/Adele_Goldstine
    QLatin1String("goldstine"),

    // Shafi Goldwasser is a computer scientist known for creating theoretical foundations of modern cryptography. Winner of 2012 ACM Turing Award.
    // https://en.wikipedia.org/wiki/Shafi_Goldwasser
    QLatin1String("goldwasser"),

    // James Golick, all around gangster.
    QLatin1String("golick"),

    // Jane Goodall - British primatologist, ethologist, and anthropologist who is considered to be the world's foremost expert on chimpanzees -
    // https://en.wikipedia.org/wiki/Jane_Goodall
    QLatin1String("goodall"),

    // Stephen Jay Gould was was an American paleontologist, evolutionary biologist, and historian of science. He is most famous for the theory of punctuated
    // equilibrium - https://en.wikipedia.org/wiki/Stephen_Jay_Gould
    QLatin1String("gould"),

    // Carolyn Widney Greider - American molecular biologist and joint winner of the 2009 Nobel Prize for Physiology or Medicine for the discovery of
    // telomerase. https://en.wikipedia.org/wiki/Carol_W._Greider
    QLatin1String("greider"),

    // Alexander Grothendieck - German-born French mathematician who became a leading figure in the creation of modern algebraic geometry.
    // https://en.wikipedia.org/wiki/Alexander_Grothendieck
    QLatin1String("grothendieck"),

    // Lois Haibt - American computer scientist, part of the team at IBM that developed FORTRAN - https://en.wikipedia.org/wiki/Lois_Haibt
    QLatin1String("haibt"),

    // Margaret Hamilton - Director of the Software Engineering Division of the MIT Instrumentation Laboratory, which developed on-board flight software for the
    // Apollo space program. https://en.wikipedia.org/wiki/Margaret_Hamilton_(scientist)
    QLatin1String("hamilton"),

    // Caroline Harriet Haslett - English electrical engineer, electricity industry administrator and champion of women's rights. Co-author of British Standard
    // 1363 that specifies AC power plugs and sockets used across the United Kingdom (which is widely considered as one of the safest designs).
    // https://en.wikipedia.org/wiki/Caroline_Haslett
    QLatin1String("haslett"),

    // Stephen Hawking pioneered the field of cosmology by combining general relativity and quantum mechanics. https://en.wikipedia.org/wiki/Stephen_Hawking
    QLatin1String("hawking"),

    // Martin Edward Hellman - American cryptologist, best known for his invention of public-key cryptography in co-operation with Whitfield Diffie and Ralph
    // Merkle. https://en.wikipedia.org/wiki/Martin_Hellman
    QLatin1String("hellman"),

    // Werner Heisenberg was a founding father of quantum mechanics. https://en.wikipedia.org/wiki/Werner_Heisenberg
    QLatin1String("heisenberg"),

    // Grete Hermann was a German philosopher noted for her philosophical work on the foundations of quantum mechanics.
    // https://en.wikipedia.org/wiki/Grete_Hermann
    QLatin1String("hermann"),

    // Caroline Lucretia Herschel - German astronomer and discoverer of several comets. https://en.wikipedia.org/wiki/Caroline_Herschel
    QLatin1String("herschel"),

    // Heinrich Rudolf Hertz - German physicist who first conclusively proved the existence of the electromagnetic waves.
    // https://en.wikipedia.org/wiki/Heinrich_Hertz
    QLatin1String("hertz"),

    // Jaroslav Heyrovský was the inventor of the polarographic method, father of the electroanalytical method, and recipient of the Nobel Prize in 1959. His
    // main field of work was polarography. https://en.wikipedia.org/wiki/Jaroslav_Heyrovsk%C3%BD
    QLatin1String("heyrovsky"),

    // Dorothy Hodgkin was a British biochemist, credited with the development of protein crystallography. She was awarded the Nobel Prize in Chemistry in 1964.
    // https://en.wikipedia.org/wiki/Dorothy_Hodgkin
    QLatin1String("hodgkin"),

    // Douglas R. Hofstadter is an American professor of cognitive science and author of the Pulitzer Prize and American Book Award-winning work Goedel, Escher,
    // Bach: An Eternal Golden Braid in 1979. A mind-bending work which coined Hofstadter's Law: QLatin1String("It always takes longer than you expect, even
    // when you take into account Hofstadter's Law." https://en.wikipedia.org/wiki/Douglas_Hofstadter
    QLatin1String("hofstadter"),

    // Erna Schneider Hoover revolutionized modern communication by inventing a computerized telephone switching method.
    // https://en.wikipedia.org/wiki/Erna_Schneider_Hoover
    QLatin1String("hoover"),

    // Grace Hopper developed the first compiler for a computer programming language and  is credited with popularizing the term QLatin1String("debugging" for
    // fixing computer glitches. https://en.wikipedia.org/wiki/Grace_Hopper
    QLatin1String("hopper"),

    // Frances Hugle, she was an American scientist, engineer, and inventor who contributed to the understanding of semiconductors, integrated circuitry, and
    // the unique electrical principles of microscopic materials. https://en.wikipedia.org/wiki/Frances_Hugle
    QLatin1String("hugle"),

    // Hypatia - Greek Alexandrine Neoplatonist philosopher in Egypt who was one of the earliest mothers of mathematics - https://en.wikipedia.org/wiki/Hypatia
    QLatin1String("hypatia"),

    // Teruko Ishizaka - Japanese scientist and immunologist who co-discovered the antibody class Immunoglobulin E.
    // https://en.wikipedia.org/wiki/Teruko_Ishizaka
    QLatin1String("ishizaka"),

    // Mary Jackson, American mathematician and aerospace engineer who earned the highest title within NASA's engineering department -
    // https://en.wikipedia.org/wiki/Mary_Jackson_(engineer)
    QLatin1String("jackson"),

    // Yeong-Sil Jang was a Korean scientist and astronomer during the Joseon Dynasty; he invented the first metal printing press and water gauge.
    // https://en.wikipedia.org/wiki/Jang_Yeong-sil
    QLatin1String("jang"),

    // Mae Carol Jemison -  is an American engineer, physician, and former NASA astronaut. She became the first black woman to travel in space when she served
    // as a mission specialist aboard the Space Shuttle Endeavour - https://en.wikipedia.org/wiki/Mae_Jemison
    QLatin1String("jemison"),

    // Betty Jennings - one of the original programmers of the ENIAC. https://en.wikipedia.org/wiki/ENIAC - https://en.wikipedia.org/wiki/Jean_Bartik
    QLatin1String("jennings"),

    // Mary Lou Jepsen, was the founder and chief technology officer of One Laptop Per Child (OLPC), and the founder of Pixel Qi.
    // https://en.wikipedia.org/wiki/Mary_Lou_Jepsen
    QLatin1String("jepsen"),

    // Katherine Coleman Goble Johnson - American physicist and mathematician contributed to the NASA. https://en.wikipedia.org/wiki/Katherine_Johnson
    QLatin1String("johnson"),

    // Irène Joliot-Curie - French scientist who was awarded the Nobel Prize for Chemistry in 1935. Daughter of Marie and Pierre Curie.
    // https://en.wikipedia.org/wiki/Ir%C3%A8ne_Joliot-Curie
    QLatin1String("joliot"),

    // Karen Spärck Jones came up with the concept of inverse document frequency, which is used in most search engines today.
    // https://en.wikipedia.org/wiki/Karen_Sp%C3%A4rck_Jones
    QLatin1String("jones"),

    // A. P. J. Abdul Kalam - is an Indian scientist aka Missile Man of India for his work on the development of ballistic missile and launch vehicle technology
    // - https://en.wikipedia.org/wiki/A._P._J._Abdul_Kalam
    QLatin1String("kalam"),

    // Sergey Petrovich Kapitsa (Russian: Серге́й Петро́вич Капи́ца; 14 February 1928 – 14 August 2012) was a Russian physicist and demographer. He was best known
    // as host of the popular and long-running Russian scientific TV show, Evident, but Incredible. His father was the Nobel laureate Soviet-era physicist Pyotr
    // Kapitsa, and his brother was the geographer and Antarctic explorer Andrey Kapitsa. - https://en.wikipedia.org/wiki/Sergey_Kapitsa
    QLatin1String("kapitsa"),

    // Susan Kare, created the icons and many of the interface elements for the original Apple Macintosh in the 1980s, and was an original employee of NeXT,
    // working as the Creative Director. https://en.wikipedia.org/wiki/Susan_Kare
    QLatin1String("kare"),

    // Mstislav Keldysh - a Soviet scientist in the field of mathematics and mechanics, academician of the USSR Academy of Sciences (1946), President of the
    // USSR Academy of Sciences (1961–1975), three times Hero of Socialist Labor (1956, 1961, 1971), fellow of the Royal Society of Edinburgh (1968).
    // https://en.wikipedia.org/wiki/Mstislav_Keldysh
    QLatin1String("keldysh"),

    // Mary Kenneth Keller, Sister Mary Kenneth Keller became the first American woman to earn a PhD in Computer Science in 1965.
    // https://en.wikipedia.org/wiki/Mary_Kenneth_Keller
    QLatin1String("keller"),

    // Johannes Kepler, German astronomer known for his three laws of planetary motion - https://en.wikipedia.org/wiki/Johannes_Kepler
    QLatin1String("kepler"),

    // Omar Khayyam - Persian mathematician, astronomer and poet. Known for his work on the classification and solution of cubic equations, for his contribution
    // to the understanding of Euclid's fifth postulate and for computing the length of a year very accurately. https://en.wikipedia.org/wiki/Omar_Khayyam
    QLatin1String("khayyam"),

    // Har Gobind Khorana - Indian-American biochemist who shared the 1968 Nobel Prize for Physiology - https://en.wikipedia.org/wiki/Har_Gobind_Khorana
    QLatin1String("khorana"),

    // Jack Kilby invented silicon integrated circuits and gave Silicon Valley its name. - https://en.wikipedia.org/wiki/Jack_Kilby
    QLatin1String("kilby"),

    // Maria Kirch - German astronomer and first woman to discover a comet - https://en.wikipedia.org/wiki/Maria_Margarethe_Kirch
    QLatin1String("kirch"),

    // Donald Knuth - American computer scientist, author of QLatin1String("The Art of Computer Programming" and creator of the TeX typesetting system.
    // https://en.wikipedia.org/wiki/Donald_Knuth
    QLatin1String("knuth"),

    // Sophie Kowalevski - Russian mathematician responsible for important original contributions to analysis, differential equations and mechanics -
    // https://en.wikipedia.org/wiki/Sofia_Kovalevskaya
    QLatin1String("kowalevski"),

    // Marie-Jeanne de Lalande - French astronomer, mathematician and cataloguer of stars - https://en.wikipedia.org/wiki/Marie-Jeanne_de_Lalande
    QLatin1String("lalande"),

    // Hedy Lamarr - Actress and inventor. The principles of her work are now incorporated into modern Wi-Fi, CDMA and Bluetooth technology.
    // https://en.wikipedia.org/wiki/Hedy_Lamarr
    QLatin1String("lamarr"),

    // Leslie B. Lamport - American computer scientist. Lamport is best known for his seminal work in distributed systems and was the winner of the 2013 Turing
    // Award. https://en.wikipedia.org/wiki/Leslie_Lamport
    QLatin1String("lamport"),

    // Mary Leakey - British paleoanthropologist who discovered the first fossilized Proconsul skull - https://en.wikipedia.org/wiki/Mary_Leakey
    QLatin1String("leakey"),

    // Henrietta Swan Leavitt - she was an American astronomer who discovered the relation between the luminosity and the period of Cepheid variable stars.
    // https://en.wikipedia.org/wiki/Henrietta_Swan_Leavitt
    QLatin1String("leavitt"),

    // Esther Miriam Zimmer Lederberg - American microbiologist and a pioneer of bacterial genetics. https://en.wikipedia.org/wiki/Esther_Lederberg
    QLatin1String("lederberg"),

    // Inge Lehmann - Danish seismologist and geophysicist. Known for discovering in 1936 that the Earth has a solid inner core inside a molten outer core.
    // https://en.wikipedia.org/wiki/Inge_Lehmann
    QLatin1String("lehmann"),

    // Daniel Lewin - Mathematician, Akamai co-founder, soldier, 9/11 victim-- Developed optimization techniques for routing traffic on the internet. Died
    // attempting to stop the 9-11 hijackers. https://en.wikipedia.org/wiki/Daniel_Lewin
    QLatin1String("lewin"),

    // Ruth Lichterman - one of the original programmers of the ENIAC. https://en.wikipedia.org/wiki/ENIAC - https://en.wikipedia.org/wiki/Ruth_Teitelbaum
    QLatin1String("lichterman"),

    // Barbara Liskov - co-developed the Liskov substitution principle. Liskov was also the winner of the Turing Prize in 2008. -
    // https://en.wikipedia.org/wiki/Barbara_Liskov
    QLatin1String("liskov"),

    // Ada Lovelace invented the first algorithm. https://en.wikipedia.org/wiki/Ada_Lovelace (thanks James Turnbull)
    QLatin1String("lovelace"),

    // Auguste and Louis Lumière - the first filmmakers in history - https://en.wikipedia.org/wiki/Auguste_and_Louis_Lumi%C3%A8re
    QLatin1String("lumiere"),

    // Mahavira - Ancient Indian mathematician during 9th century AD who discovered basic algebraic identities -
    // https://en.wikipedia.org/wiki/Mah%C4%81v%C4%ABra_(mathematician)
    QLatin1String("mahavira"),

    // Lynn Margulis (b. Lynn Petra Alexander) - an American evolutionary theorist and biologist, science author, educator, and popularizer, and was the primary
    // modern proponent for the significance of symbiosis in evolution. - https://en.wikipedia.org/wiki/Lynn_Margulis
    QLatin1String("margulis"),

    // Yukihiro Matsumoto - Japanese computer scientist and software programmer best known as the chief designer of the Ruby programming language.
    // https://en.wikipedia.org/wiki/Yukihiro_Matsumoto
    QLatin1String("matsumoto"),

    // James Clerk Maxwell - Scottish physicist, best known for his formulation of electromagnetic theory. https://en.wikipedia.org/wiki/James_Clerk_Maxwell
    QLatin1String("maxwell"),

    // Maria Mayer - American theoretical physicist and Nobel laureate in Physics for proposing the nuclear shell model of the atomic nucleus -
    // https://en.wikipedia.org/wiki/Maria_Mayer
    QLatin1String("mayer"),

    // John McCarthy invented LISP: https://en.wikipedia.org/wiki/John_McCarthy_(computer_scientist)
    QLatin1String("mccarthy"),

    // Barbara McClintock - a distinguished American cytogeneticist, 1983 Nobel Laureate in Physiology or Medicine for discovering transposons.
    // https://en.wikipedia.org/wiki/Barbara_McClintock
    QLatin1String("mcclintock"),

    // Anne Laura Dorinthea McLaren - British developmental biologist whose work helped lead to human in-vitro fertilisation.
    // https://en.wikipedia.org/wiki/Anne_McLaren
    QLatin1String("mclaren"),

    // Malcolm McLean invented the modern shipping container: https://en.wikipedia.org/wiki/Malcom_McLean
    QLatin1String("mclean"),

    // Kay McNulty - one of the original programmers of the ENIAC. https://en.wikipedia.org/wiki/ENIAC - https://en.wikipedia.org/wiki/Kathleen_Antonelli
    QLatin1String("mcnulty"),

    // Gregor Johann Mendel - Czech scientist and founder of genetics. https://en.wikipedia.org/wiki/Gregor_Mendel
    QLatin1String("mendel"),

    // Dmitri Mendeleev - a chemist and inventor. He formulated the Periodic Law, created a farsighted version of the periodic table of elements, and used it to
    // correct the properties of some already discovered elements and also to predict the properties of eight elements yet to be discovered.
    // https://en.wikipedia.org/wiki/Dmitri_Mendeleev
    QLatin1String("mendeleev"),

    // Lise Meitner - Austrian/Swedish physicist who was involved in the discovery of nuclear fission. The element meitnerium is named after her -
    // https://en.wikipedia.org/wiki/Lise_Meitner
    QLatin1String("meitner"),

    // Carla Meninsky, was the game designer and programmer for Atari 2600 games Dodge 'Em and Warlords. https://en.wikipedia.org/wiki/Carla_Meninsky
    QLatin1String("meninsky"),

    // Ralph C. Merkle - American computer scientist, known for devising Merkle's puzzles - one of the very first schemes for public-key cryptography. Also,
    // inventor of Merkle trees and co-inventor of the Merkle-Damgård construction for building collision-resistant cryptographic hash functions and the
    // Merkle-Hellman knapsack cryptosystem. https://en.wikipedia.org/wiki/Ralph_Merkle
    QLatin1String("merkle"),

    // Johanna Mestorf - German prehistoric archaeologist and first female museum director in Germany - https://en.wikipedia.org/wiki/Johanna_Mestorf
    QLatin1String("mestorf"),

    // Maryam Mirzakhani - an Iranian mathematician and the first woman to win the Fields Medal. https://en.wikipedia.org/wiki/Maryam_Mirzakhani
    QLatin1String("mirzakhani"),

    // Rita Levi-Montalcini - Won Nobel Prize in Physiology or Medicine jointly with colleague Stanley Cohen for the discovery of nerve growth factor
    // (https://en.wikipedia.org/wiki/Rita_Levi-Montalcini)
    QLatin1String("montalcini"),

    // Gordon Earle Moore - American engineer, Silicon Valley founding father, author of Moore's law. https://en.wikipedia.org/wiki/Gordon_Moore
    QLatin1String("moore"),

    // Samuel Morse - contributed to the invention of a single-wire telegraph system based on European telegraphs and was a co-developer of the Morse code -
    // https://en.wikipedia.org/wiki/Samuel_Morse
    QLatin1String("morse"),

    // Ian Murdock - founder of the Debian project - https://en.wikipedia.org/wiki/Ian_Murdock
    QLatin1String("murdock"),

    // May-Britt Moser - Nobel prize winner neuroscientist who contributed to the discovery of grid cells in the brain.
    // https://en.wikipedia.org/wiki/May-Britt_Moser
    QLatin1String("moser"),

    // John Napier of Merchiston - Scottish landowner known as an astronomer, mathematician and physicist. Best known for his discovery of logarithms.
    // https://en.wikipedia.org/wiki/John_Napier
    QLatin1String("napier"),

    // John Forbes Nash, Jr. - American mathematician who made fundamental contributions to game theory, differential geometry, and the study of partial
    // differential equations. https://en.wikipedia.org/wiki/John_Forbes_Nash_Jr.
    QLatin1String("nash"),

    // John von Neumann - todays computer architectures are based on the von Neumann architecture. https://en.wikipedia.org/wiki/Von_Neumann_architecture
    QLatin1String("neumann"),

    // Isaac Newton invented classic mechanics and modern optics. https://en.wikipedia.org/wiki/Isaac_Newton
    QLatin1String("newton"),

    // Florence Nightingale, more prominently known as a nurse, was also the first female member of the Royal Statistical Society and a pioneer in statistical
    // graphics https://en.wikipedia.org/wiki/Florence_Nightingale#Statistics_and_sanitary_reform
    QLatin1String("nightingale"),

    // Alfred Nobel - a Swedish chemist, engineer, innovator, and armaments manufacturer (inventor of dynamite) - https://en.wikipedia.org/wiki/Alfred_Nobel
    QLatin1String("nobel"),

    // Emmy Noether, German mathematician. Noether's Theorem is named after her. https://en.wikipedia.org/wiki/Emmy_Noether
    QLatin1String("noether"),

    // Poppy Northcutt. Poppy Northcutt was the first woman to work as part of NASA’s Mission Control.
    // http://www.businessinsider.com/poppy-northcutt-helped-apollo-astronauts-2014-12?op=1
    QLatin1String("northcutt"),

    // Robert Noyce invented silicon integrated circuits and gave Silicon Valley its name. - https://en.wikipedia.org/wiki/Robert_Noyce
    QLatin1String("noyce"),

    // Panini - Ancient Indian linguist and grammarian from 4th century CE who worked on the world's first formal system -
    // https://en.wikipedia.org/wiki/P%C4%81%E1%B9%87ini#Comparison_with_modern_formal_systems
    QLatin1String("panini"),

    // Ambroise Pare invented modern surgery. https://en.wikipedia.org/wiki/Ambroise_Par%C3%A9
    QLatin1String("pare"),

    // Blaise Pascal, French mathematician, physicist, and inventor - https://en.wikipedia.org/wiki/Blaise_Pascal
    QLatin1String("pascal"),

    // Louis Pasteur discovered vaccination, fermentation and pasteurization. https://en.wikipedia.org/wiki/Louis_Pasteur.
    QLatin1String("pasteur"),

    // Cecilia Payne-Gaposchkin was an astronomer and astrophysicist who, in 1925, proposed in her Ph.D. thesis an explanation for the composition of stars in
    // terms of the relative abundances of hydrogen and helium. https://en.wikipedia.org/wiki/Cecilia_Payne-Gaposchkin
    QLatin1String("payne"),

    // Radia Perlman is a software designer and network engineer and most famous for her invention of the spanning-tree protocol (STP).
    // https://en.wikipedia.org/wiki/Radia_Perlman
    QLatin1String("perlman"),

    // Rob Pike was a key contributor to Unix, Plan 9, the X graphic system, utf-8, and the Go programming language. https://en.wikipedia.org/wiki/Rob_Pike
    QLatin1String("pike"),

    // Henri Poincaré made fundamental contributions in several fields of mathematics. https://en.wikipedia.org/wiki/Henri_Poincar%C3%A9
    QLatin1String("poincare"),

    // Laura Poitras is a director and producer whose work, made possible by open source crypto tools, advances the causes of truth and freedom of information
    // by reporting disclosures by whistleblowers such as Edward Snowden. https://en.wikipedia.org/wiki/Laura_Poitras
    QLatin1String("poitras"),

    // Tat’yana Avenirovna Proskuriakova (Russian: Татья́на Авени́ровна Проскуряко́ва) (January 23 [O.S. January 10] 1909 – August 30, 1985) was a Russian-American
    // Mayanist scholar and archaeologist who contributed significantly to the deciphering of Maya hieroglyphs, the writing system of the pre-Columbian Maya
    // civilization of Mesoamerica. https://en.wikipedia.org/wiki/Tatiana_Proskouriakoff
    QLatin1String("proskuriakova"),

    // Claudius Ptolemy - a Greco-Egyptian writer of Alexandria, known as a mathematician, astronomer, geographer, astrologer, and poet of a single epigram in
    // the Greek Anthology - https://en.wikipedia.org/wiki/Ptolemy
    QLatin1String("ptolemy"),

    // C. V. Raman - Indian physicist who won the Nobel Prize in 1930 for proposing the Raman effect. - https://en.wikipedia.org/wiki/C._V._Raman
    QLatin1String("raman"),

    // Srinivasa Ramanujan - Indian mathematician and autodidact who made extraordinary contributions to mathematical analysis, number theory, infinite series,
    // and continued fractions. - https://en.wikipedia.org/wiki/Srinivasa_Ramanujan
    QLatin1String("ramanujan"),

    // Sally Kristen Ride was an American physicist and astronaut. She was the first American woman in space, and the youngest American astronaut.
    // https://en.wikipedia.org/wiki/Sally_Ride
    QLatin1String("ride"),

    // Dennis Ritchie - co-creator of UNIX and the C programming language. - https://en.wikipedia.org/wiki/Dennis_Ritchie
    QLatin1String("ritchie"),

    // Ida Rhodes - American pioneer in computer programming, designed the first computer used for Social Security. https://en.wikipedia.org/wiki/Ida_Rhodes
    QLatin1String("rhodes"),

    // Julia Hall Bowman Robinson - American mathematician renowned for her contributions to the fields of computability theory and computational complexity
    // theory. https://en.wikipedia.org/wiki/Julia_Robinson
    QLatin1String("robinson"),

    // Wilhelm Conrad Röntgen - German physicist who was awarded the first Nobel Prize in Physics in 1901 for the discovery of X-rays (Röntgen rays).
    // https://en.wikipedia.org/wiki/Wilhelm_R%C3%B6ntgen
    QLatin1String("roentgen"),

    // Rosalind Franklin - British biophysicist and X-ray crystallographer whose research was critical to the understanding of DNA -
    // https://en.wikipedia.org/wiki/Rosalind_Franklin
    QLatin1String("rosalind"),

    // Vera Rubin - American astronomer who pioneered work on galaxy rotation rates. https://en.wikipedia.org/wiki/Vera_Rubin
    QLatin1String("rubin"),

    // Meghnad Saha - Indian astrophysicist best known for his development of the Saha equation, used to describe chemical and physical conditions in stars -
    // https://en.wikipedia.org/wiki/Meghnad_Saha
    QLatin1String("saha"),

    // Jean E. Sammet developed FORMAC, the first widely used computer language for symbolic manipulation of mathematical formulas.
    // https://en.wikipedia.org/wiki/Jean_E._Sammet
    QLatin1String("sammet"),

    // Mildred Sanderson - American mathematician best known for Sanderson's theorem concerning modular invariants.
    // https://en.wikipedia.org/wiki/Mildred_Sanderson
    QLatin1String("sanderson"),

    // Satoshi Nakamoto is the name used by the unknown person or group of people who developed bitcoin, authored the bitcoin white paper, and created and
    // deployed bitcoin's original reference implementation. https://en.wikipedia.org/wiki/Satoshi_Nakamoto
    QLatin1String("satoshi"),

    // Adi Shamir - Israeli cryptographer whose numerous inventions and contributions to cryptography include the Ferge Fiat Shamir identification scheme, the
    // Rivest Shamir Adleman (RSA) public-key cryptosystem, the Shamir's secret sharing scheme, the breaking of the Merkle-Hellman cryptosystem, the TWINKLE and
    // TWIRL factoring devices and the discovery of differential cryptanalysis (with Eli Biham). https://en.wikipedia.org/wiki/Adi_Shamir
    QLatin1String("shamir"),

    // Claude Shannon - The father of information theory and founder of digital circuit design theory. (https://en.wikipedia.org/wiki/Claude_Shannon)
    QLatin1String("shannon"),

    // Carol Shaw - Originally an Atari employee, Carol Shaw is said to be the first female video game designer.
    // https://en.wikipedia.org/wiki/Carol_Shaw_(video_game_designer)
    QLatin1String("shaw"),

    // Dame Stephanie QLatin1String("Steve" Shirley - Founded a software company in 1962 employing women working from home.
    // https://en.wikipedia.org/wiki/Steve_Shirley
    QLatin1String("shirley"),

    // William Shockley co-invented the transistor - https://en.wikipedia.org/wiki/William_Shockley
    QLatin1String("shockley"),

    // Lina Solomonovna Stern (or Shtern; Russian: Лина Соломоновна Штерн; 26 August 1878 – 7 March 1968) was a Soviet biochemist, physiologist and humanist
    // whose medical discoveries saved thousands of lives at the fronts of World War II. She is best known for her pioneering work on blood–brain barrier, which
    // she described as hemato-encephalic barrier in 1921. https://en.wikipedia.org/wiki/Lina_Stern
    QLatin1String("shtern"),

    // Françoise Barré-Sinoussi - French virologist and Nobel Prize Laureate in Physiology or Medicine; her work was fundamental in identifying HIV as the cause
    // of AIDS. https://en.wikipedia.org/wiki/Fran%C3%A7oise_Barr%C3%A9-Sinoussi
    QLatin1String("sinoussi"),

    // Betty Snyder - one of the original programmers of the ENIAC. https://en.wikipedia.org/wiki/ENIAC - https://en.wikipedia.org/wiki/Betty_Holberton
    QLatin1String("snyder"),

    // Cynthia Solomon - Pioneer in the fields of artificial intelligence, computer science and educational computing. Known for creation of Logo, an
    // educational programming language.  https://en.wikipedia.org/wiki/Cynthia_Solomon
    QLatin1String("solomon"),

    // Frances Spence - one of the original programmers of the ENIAC. https://en.wikipedia.org/wiki/ENIAC - https://en.wikipedia.org/wiki/Frances_Spence
    QLatin1String("spence"),

    // Michael Stonebraker is a database research pioneer and architect of Ingres, Postgres, VoltDB and SciDB. Winner of 2014 ACM Turing Award.
    // https://en.wikipedia.org/wiki/Michael_Stonebraker
    QLatin1String("stonebraker"),

    // Ivan Edward Sutherland - American computer scientist and Internet pioneer, widely regarded as the father of computer graphics.
    // https://en.wikipedia.org/wiki/Ivan_Sutherland
    QLatin1String("sutherland"),

    // Janese Swanson (with others) developed the first of the Carmen Sandiego games. She went on to found Girl Tech.
    // https://en.wikipedia.org/wiki/Janese_Swanson
    QLatin1String("swanson"),

    // Aaron Swartz was influential in creating RSS, Markdown, Creative Commons, Reddit, and much of the internet as we know it today. He was devoted to freedom
    // of information on the web. https://en.wikiquote.org/wiki/Aaron_Swartz
    QLatin1String("swartz"),

    // Bertha Swirles was a theoretical physicist who made a number of contributions to early quantum theory. https://en.wikipedia.org/wiki/Bertha_Swirles
    QLatin1String("swirles"),

    // Helen Brooke Taussig - American cardiologist and founder of the field of paediatric cardiology. https://en.wikipedia.org/wiki/Helen_B._Taussig
    QLatin1String("taussig"),

    // Valentina Tereshkova is a Russian engineer, cosmonaut and politician. She was the first woman to fly to space in 1963. In 2013, at the age of 76, she
    // offered to go on a one-way mission to Mars. https://en.wikipedia.org/wiki/Valentina_Tereshkova
    QLatin1String("tereshkova"),

    // Nikola Tesla invented the AC electric system and every gadget ever used by a James Bond villain. https://en.wikipedia.org/wiki/Nikola_Tesla
    QLatin1String("tesla"),

    // Marie Tharp - American geologist and oceanic cartographer who co-created the first scientific map of the Atlantic Ocean floor. Her work led to the
    // acceptance of the theories of plate tectonics and continental drift. https://en.wikipedia.org/wiki/Marie_Tharp
    QLatin1String("tharp"),

    // Ken Thompson - co-creator of UNIX and the C programming language - https://en.wikipedia.org/wiki/Ken_Thompson
    QLatin1String("thompson"),

    // Linus Torvalds invented Linux and Git. https://en.wikipedia.org/wiki/Linus_Torvalds
    QLatin1String("torvalds"),

    // Youyou Tu - Chinese pharmaceutical chemist and educator known for discovering artemisinin and dihydroartemisinin, used to treat malaria, which has saved
    // millions of lives. Joint winner of the 2015 Nobel Prize in Physiology or Medicine. https://en.wikipedia.org/wiki/Tu_Youyou
    QLatin1String("tu"),

    // Alan Turing was a founding father of computer science. https://en.wikipedia.org/wiki/Alan_Turing.
    QLatin1String("turing"),

    // Varahamihira - Ancient Indian mathematician who discovered trigonometric formulae during 505-587 CE -
    // https://en.wikipedia.org/wiki/Var%C4%81hamihira#Contributions
    QLatin1String("varahamihira"),

    // Dorothy Vaughan was a NASA mathematician and computer programmer on the SCOUT launch vehicle program that put America's first satellites into space -
    // https://en.wikipedia.org/wiki/Dorothy_Vaughan
    QLatin1String("vaughan"),

    // Cédric Villani - French mathematician, won Fields Medal, Fermat Prize and Poincaré Price for his work in differential geometry and statistical mechanics.
    // https://en.wikipedia.org/wiki/C%C3%A9dric_Villani
    QLatin1String("villani"),

    // Sir Mokshagundam Visvesvaraya - is a notable Indian engineer.  He is a recipient of the Indian Republic's highest honour, the Bharat Ratna, in 1955. On
    // his birthday, 15 September is celebrated as Engineer's Day in India in his memory - https://en.wikipedia.org/wiki/Visvesvaraya
    QLatin1String("visvesvaraya"),

    // Christiane Nüsslein-Volhard - German biologist, won Nobel Prize in Physiology or Medicine in 1995 for research on the genetic control of embryonic
    // development. https://en.wikipedia.org/wiki/Christiane_N%C3%BCsslein-Volhard
    QLatin1String("volhard"),

    // Marlyn Wescoff - one of the original programmers of the ENIAC. https://en.wikipedia.org/wiki/ENIAC - https://en.wikipedia.org/wiki/Marlyn_Meltzer
    QLatin1String("wescoff"),

    // Sylvia B. Wilbur - British computer scientist who helped develop the ARPANET, was one of the first to exchange email in the UK and a leading researcher
    // in computer-supported collaborative work. https://en.wikipedia.org/wiki/Sylvia_Wilbur
    QLatin1String("wilbur"),

    // Andrew Wiles - Notable British mathematician who proved the enigmatic Fermat's Last Theorem - https://en.wikipedia.org/wiki/Andrew_Wiles
    QLatin1String("wiles"),

    // Roberta Williams, did pioneering work in graphical adventure games for personal computers, particularly the King's Quest series.
    // https://en.wikipedia.org/wiki/Roberta_Williams
    QLatin1String("williams"),

    // Malcolm John Williamson - British mathematician and cryptographer employed by the GCHQ. Developed in 1974 what is now known as Diffie-Hellman key
    // exchange (Diffie and Hellman first published the scheme in 1976). https://en.wikipedia.org/wiki/Malcolm_J._Williamson
    QLatin1String("williamson"),

    // Sophie Wilson designed the first Acorn Micro-Computer and the instruction set for ARM processors. https://en.wikipedia.org/wiki/Sophie_Wilson
    QLatin1String("wilson"),

    // Jeannette Wing - co-developed the Liskov substitution principle. - https://en.wikipedia.org/wiki/Jeannette_Wing
    QLatin1String("wing"),

    // Steve Wozniak invented the Apple I and Apple II. https://en.wikipedia.org/wiki/Steve_Wozniak
    QLatin1String("wozniak"),

    // The Wright brothers, Orville and Wilbur - credited with inventing and building the world's first successful airplane and making the first controlled,
    // powered and sustained heavier-than-air human flight - https://en.wikipedia.org/wiki/Wright_brothers
    QLatin1String("wright"),

    // Chien-Shiung Wu - Chinese-American experimental physicist who made significant contributions to nuclear physics.
    // https://en.wikipedia.org/wiki/Chien-Shiung_Wu
    QLatin1String("wu"),

    // Rosalyn Sussman Yalow - Rosalyn Sussman Yalow was an American medical physicist, and a co-winner of the 1977 Nobel Prize in Physiology or Medicine for
    // development of the radioimmunoassay technique. https://en.wikipedia.org/wiki/Rosalyn_Sussman_Yalow
    QLatin1String("yalow"),

    // Ada Yonath - an Israeli crystallographer, the first woman from the Middle East to win a Nobel prize in the sciences.
    // https://en.wikipedia.org/wiki/Ada_Yonath
    QLatin1String("yonath"),

    // Nikolay Yegorovich Zhukovsky (Russian: Никола́й Его́рович Жуко́вский, January 17 1847 – March 17, 1921) was a Russian scientist, mathematician and engineer,
    // and a founding father of modern aero- and hydrodynamics. Whereas contemporary scientists scoffed at the idea of human flight, Zhukovsky was the first to
    // undertake the study of airflow. He is often called the Father of Russian Aviation. https://en.wikipedia.org/wiki/Nikolay_Yegorovich_Zhukovsky
    QLatin1String("zhukovsky"),
};

#include <random>

QString getRandomName()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> l_distrib(0, left_names.size() - 1);
    std::uniform_int_distribution<> r_distrib(0, right_names.size() - 1);

    return left_names[l_distrib(gen)] + QLatin1String(" ") + right_names[r_distrib(gen)];
}
