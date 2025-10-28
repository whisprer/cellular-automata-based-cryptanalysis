Folder PATH listing for volume X:/
Volume serial number is X0XX-X00X
X:.
|   .gitattributes
|   .gitignore
|   CMakeLists.txt
|   DECLARATION.md
|   desktop.ini
|   filelist.txt
|   LICENCE
|   README
|   filelist.md 

|  
+---code
|

+generators

|   + ---tests
|          +---nist\_sts

|          |   generator\_factory.cpp

|          |\_ generator\_factory.hpp
|  
+ ---src
|   bitsequence.cpp
|   bitsequence.hpp
|   caca\_test.cpp
|  
|                 
+------------v-NIST\_test-suite
|                       approximate\_entropy\_test.cpp
|                       approximate\_entropy\_test.hpp
|                       bbs.cpp
|                       bbs.hpp
|                       bitsequence.cpp
|                       bitsequence.hpp
|                       block\_frequency\_test.cpp
|                       block\_frequency\_test.hpp
|                       ca\_analyzer.cpp
|                       ca\_analyzer.hpp
|                       common.cpp
|                       common.hpp
|                       core\_class\_structure.cpp
|                       cubic\_congruent.cpp
|                       cubic\_congruent.hpp
|                       cumulative\_sums\_test.cpp
|                       cumulative\_sums\_test.hpp
|                       dft\_test.cpp
|                       dft\_test.hpp
|                       example\_usage.cpp
|                       frequency.cpp
|                       frequency\_test.cpp
|                       frequency\_test.hpp
|                       generator\_factory.cpp
|                       generator\_factory.hpp
|                       hath\_functions.hpp
|                       lcg.cpp
|                       lcg.hpp
|                       lcgA.cpp
|                       lcgb.cpp
|                       linear\_complexity\_test.cpp
|                       linear\_complexity\_test.hpp
|                       longest\_run\_test.cpp
|                       longest\_run\_test.hpp
|                       main.cpp
|                       math\_fucntions.cpp
|                       micali\_schorr.cpp
|                       mical\_schorr.hpp
|                       modular\_exponentiation.cpp
|                       modular\_exponentiation.hpp
|                       nist\_sts.hpp
|                       non\_overlapping\_template\_test.cpp
|                       non\_overlapping\_template\_test.hpp
|                       overlapping\_template\_test.cpp
|                       overlapping\_template\_test.hpp
|                       quadratic\_congruential.cpp
|                       quadratic\_congruential.hpp
|                       random\_excursions\_test.cpp
|                       random\_excursions\_test.hpp
|                       random\_excursions\_variant\_test.cpp
|                       random\_excursions\_variant\_test.hpp
|                       random\_number\_generator.hpp
|                       rank\_test.cpp
|                       rank\_trest.hpp
|                       runs\_test.hpp
|                       serial\_test.cpp
|                       serial\_test.hpp
|                       sha1.cpp
|                       sha1.hpp
|                       stat\_analyzer.hpp
|                       test-suite..cpp
|                       test\_suite.cpp
|                       test\_suite.hpp
|                       universal\_test.cpp
|                       universal\_test.hpp
|                       xor\_generator.cpp.cpp
|                       xor\_generator.hpp
|  
+---docs
|       .~lock.refs\_litrev.odt#
|       an-improved-cellular-automata-ca-based-image-denoising-method-for-biometric-applications.pdf
|       caca\_dataset\_gen\_og.md
|       caca\_nother\_idea.md
|       expanded\_conclusions\_b.md
|       expanded\_results\_a.md
|       initial\_resullts\_overview.md
|       joined\_attempt\_i.md
|       next.md
|       NIST.IR.8446.ipd.pdf
|       nist.md
|       paper\_a.odt
|       paper\_b.odt
|       paper\_just\_needs\_refs\_plus\_code\_example.md
|       paper\_partial\_a.md
|       paper\_partial\_b.md
|       quick\_write-up.md
|       refs\_litrev.odt
|       use\_for\_the\_tech.md
|  
+---test
|   |   test\_dataset.zip
|   |  
|   +---dataset\_gen
|   |       1KB\_research\_paper.bin
|   |       500KB\_research\_paper.bin
|   |       50MB\_research\_paper.bin
|   |       caca\_dataset\_gen.md
|   |       ctrl\_noise\_5k.py
|   |       jpg\_droids\_lolly\_1.jpg
|   |       jpg\_droids\_lolly\_1\_histogram.png
|   |       ncpt\_ceasar.py
|   |       ncpt\_cheesy.py
|   |       ncpt\_flawed\_prng.py
|   |       ncpt\_shift\_mix.py
|   |       ncpt\_XOR\_short.py
|   |  
|   +---outputs
|   |   |   ca\_visuals\_pngs.zip
|   |   |   chi\_by\_dataset.png
|   |   |   controls\_dataset.zip
|   |   |   entropy\_by\_dataset.png
|   |   |   ic\_by\_dataset.png
|   |   |  
|   |   ---simple\_ca\_test\_output
|   |       |   biggest - Copy (2) - Copy.png
|   |       |   biggest - Copy (2).png
|   |       |   biggest - Copy - Copy (2).png
|   |       |   biggest - Copy - Copy - Copy.png
|   |       |   biggest - Copy - Copy.png
|   |       |   biggest - Copy.png
|   |       |   biggest.png
|   |       |   ctrl - Copy.png
|   |       |   ctrl.png
|   |       |   ctrl\_corr - Copy - Copy.png
|   |       |   ctrl\_corr - Copy.png
|   |       |   ctrl\_corr.png
|   |       |   ctrl\_heat - Copy.png
|   |       |   ctrl\_heat.png
|   |       |   ctrl\_histo - Copy.png
|   |       |   ctrl\_histo.png
|   |       |   Screenshot 2025-03-16 114530 - Copy.png
|   |       |   Screenshot 2025-03-16 114530.png
|   |       |   Screenshot 2025-03-16 114611 - Copy.png
|   |       |   Screenshot 2025-03-16 114611.png
|   |       |   Screenshot 2025-03-16 114700.png
|   |       |   Screenshot 2025-03-16 114730 - Copy.png
|   |       |   Screenshot 2025-03-16 114730.png
|   |       |   visualize\_all\_bin.py
|   |       |   visualize\_bin.py
|   |       |  
|   |       +---110
|   |       |   |   110.zip
|   |       |   |   110\_heat.png
|   |       |   |   110\_histo.png
|   |       |   |   110\_~corr.png
|   |       |   |   xor\_test\_rule110\_iter5.bin
|   |       |   |  
|   |       |   +---110\_corr
|   |       |   |       aes\_ctr\_test\_rule110\_iter10\_correlation.png
|   |       |   |       aes\_ctr\_test\_rule110\_iter1\_correlation.png
|   |       |   |       aes\_ctr\_test\_rule110\_iter3\_correlation.png
|   |       |   |       aes\_ctr\_test\_rule110\_iter5\_correlation.png
|   |       |   |       xor\_test\_rule110\_iter5\_correlation.png
|   |       |   |  
|   |       |   +---110\_heat
|   |       |   |       aes\_ecb\_test\_rule110\_iter10\_heatmap.png
|   |       |   |       aes\_ecb\_test\_rule110\_iter1\_heatmap.png
|   |       |   |       aes\_ecb\_test\_rule110\_iter3\_heatmap.png
|   |       |   |       aes\_ecb\_test\_rule110\_iter5\_heatmap.png
|   |       |   |       xor\_test\_rule110\_iter5\_heatmap.png
|   |       |   |  
|   |       |   ---110\_histo
|   |       |           caesar\_test\_rule110\_iter10\_histogram.png
|   |       |           caesar\_test\_rule110\_iter1\_histogram.png
|   |       |           caesar\_test\_rule110\_iter3\_histogram.png
|   |       |           caesar\_test\_rule110\_iter5\_histogram.png
|   |       |           xor\_test\_rule110\_iter5\_histogram.png
|   |       |  
|   |       +---150
|   |       |   |   xor\_test\_rule150\_iter1.bin
|   |       |   |   xor\_test\_rule150\_iter10.bin
|   |       |   |   xor\_test\_rule150\_iter3.bin
|   |       |   |   xor\_test\_rule150\_iter5.bin
|   |       |   |  
|   |       |   +---150\_corr
|   |       |   |       xor\_test\_rule150\_iter5\_correlation.png
|   |       |   |  
|   |       |   +---150\_heat
|   |       |   |       xor\_test\_rule150\_iter5\_heatmap.png
|   |       |   |  
|   |       |   ---150\_histo
|   |       |           xor\_test\_rule150\_iter5\_histogram.png
|   |       |  
|   |       +---30
|   |       |   |   xor\_test\_rule30\_iter5.bin
|   |       |   |  
|   |       |   +---30\_corr
|   |       |   |       xor\_test\_rule82\_iter1\_correlation.png
|   |       |   |  
|   |       |   +---30\_heat
|   |       |   |       xor\_test\_rule30\_iter5\_heatmap.png
|   |       |   |  
|   |       |   ---30\_histo
|   |       |           xor\_test\_rule30\_iter5\_histogram.png
|   |       |  
|   |       +---82
|   |       |   |   xor\_test\_rule82\_iter5.bin
|   |       |   |  
|   |       |   +---82\_corr
|   |       |   |       xor\_test\_rule82\_iter5\_correlation.png
|   |       |   |  
|   |       |   +---82\_heat
|   |       |   |       xor\_test\_rule82\_iter5\_heatmap.png
|   |       |   |  
|   |       |   ---82\_histo
|   |       |           xor\_test\_rule82\_iter5\_histogram.png
|   |       |  
|   |       ---ctrl
|   |           +---ctrl\_corr
|   |           |       1KB\_research\_paper\_correlation.png
|   |           |       log\_file\_bash\_history\_correlation.png
|   |           |       random\_noise\_5k\_ctrl\_correlation.png
|   |           |  
|   |           +---ctrl\_hea
|   |           |       1KB\_research\_paper\_heatmap.png
|   |           |       log\_file\_bash\_history\_heatmap.png
|   |           |       random\_noise\_5k\_ctrl\_heatmap.png
|   |           |  
|   |           ---ctrl\_histo
|   |                   1KB\_research\_paper\_histogram.png
|   |                   500KB\_research\_paper\_histogram.png
|   |                   log\_file\_bash\_history\_histogram.png
|   |                   random\_noise\_5k\_ctrl\_histogram.png
|   |  
|   ---test\_dataset
|           1KB-0xFF.bin
|           1KB-cheesy.bin
|           1KB-DEADBEEF.bin
|           1KB-paper\_aes\_ecb.bin
|           1KB\_paperflawrng.bin
|           1KB\_paperXORd.bin
|           1KB\_paper\_aes\_cbc\_iv.bin
|           1KB\_paper\_aes\_ctr.bin
|           1KB\_paper\_caesar.bin
|           1KB\_paper\_des.bin
|           1KB\_paper\_SHIFT\_MIXd.bin
|           500KB-0xFF.bin
|           500KB-cheesy.bin
|           500KB-DEADBEEF.bin
|           500KB-paper\_aes\_ecb.bin
|           500KB\_paperflawrng.bin
|           500KB\_paperXORd.bin
|           500KB\_paper\_aes\_cbc\_iv.bin
|           500KB\_paper\_aes\_ctr.bin
|           500KB\_paper\_caesar.bin
|           500KB\_paper\_des.bin
|           500KB\_paper\_SHIFT\_MIXd.bin
|           50MB-0xFF.bin
|           50MB-cheesy.bin
|           50MB-DEADBEEF.bin
|           50MB-paper\_aes\_ecb.bin
|           50MB\_paperflawrng.bin
|           50MB\_paperXORd.bin
|           50MB\_paper\_aes\_cbc\_iv.bin
|           50MB\_paper\_aes\_ctr.bin
|           50MB\_paper\_caesar.bin
|           50MB\_paper\_des.bin
|           50MB\_paper\_SHIFT\_MIXd.bin
|           bash\_history\_ceasar.bin
|           bash\_history\_flawedprng.bin
|           bash\_history\_shiftmx.bin
|           bash\_history\_XORd.bin
|           caca
|           caca\_fast
|           caca\_fast.exe
|           caca\_results\_rule110.bin
|           caca\_results\_rule150.bin
|           caca\_results\_rule30.bin
|           ctrl\_random\_noise\_5k.bin
|           droids\_lolly\_1\_caesar.bin
|           droids\_lolly\_1\_flawedprng.bin
|           droids\_lolly\_1\_shiftmx.bin
|           droids\_lolly\_1\_XORd.bin
|           HOW\_TO.md
|           jpg\_droids\_lolly\_1.bin
|           log\_file\_bash\_history.bin
|           run\_all.ps1
|           run\_all.sh
|  
+---v-5-0-0-
|   |   caca.exe
|   |   caca\_main.cpp
|   |  
|   ---v-4-0-0-
|       +---.vscode
|       |       settings.json
|       |  
|       ---src
|           +---generators
|           ---nist\_sts
|               ---tests
|  
---v-5-0-1
caca\_fast
caca\_fast.exe
caca\_optimized.cpp

