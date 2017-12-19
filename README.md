# Daredevil Project for a106

![Daredevil](http://s24.postimg.org/snrt08gut/image.png "Daredevil Kernel logo")

# Kernel compatible roms:

  * Lineage OS 13.0 Unofficial
  
  * Lineage OS 14.1 Unofficial

# Compilation guide:
  
  * First, made a folder for the kernel and enter to it:

        $ mkdir kernel

        $ cd kernel

  * Then, clone the project: 

        $ git clone -b cm-14.1 https://github.com/GODz-Android-Dev/DareDevil_a106.git


  * And for compile:

        * Enter to the Daredevil kernel directory 

        $ ./dd

  * The kernel file (called zImage, will be found in arch/arm/boot folder)


# Important Notice:

        * The Daredevil Kernel is already implemented on my Unofficial lineage os builds

        * The OFFICIAL linux 3.x readme can be found on the Documentation folder

        * If you use this kernel for your custom roms, please atach the git contributors and put me on the credit section


# Thanks to (In alphabetical order):

   * ANDR7E ( help with kpd buttons + device info app for mtk devices) ---- XDA, 4PDA, GITHUB
   * @assusdan help me with drivers and develop the kernel. ---- 4PDA, GITHUB
   * BQ for his sources (LCM DRVIERS, TOUCH DRIVERS, ETC) ---- GITHUB
   * @Pablito2020 (Adapted to krillin ,upstreamed and introducted new features to the kernel)
   * Vineeth Raj for his dt2wake and sweep2wake commits for sprout. ---- GITHUB
   * Varun Chitre for his "thunder" drivers for sprout. ---- GITHUB
   * @zormax for the bring up LP/MM project ---- 4PDA
