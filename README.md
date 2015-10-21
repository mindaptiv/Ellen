# Ellen  

##Abstract  
Grabs system and user information in a Linux context  

##Goal  
Ellen is a Linux (LSB supporting distros in particular) targeted library for gathering both user and system information for the current device.  In order to be compatible with a variety of Linux distros, the majority of Ellen's code relies on libraries available in the Linux Standard Base.  All other calls are dynamically loaded in via the dlfcn header.  Ellen will store all of the user information, hardware information, and OS information it can find inside of a portable struct that can be passed between operating systems or to external applications.  In cases where Ellen cannot find a certain piece of data, or the field in the cylonStruct object has no context on a Linux machine, a default value will be used.  

##Tested Distros
The following is a list of confirmed distros that Ellen was able to run successfully on:
* Ubuntu 14.04 LTS (Barebones & Marrow releases)
* Ubuntu 15.04 (Marrow release only)

##Documentation  
For more detailed information, please visit the [Ellen wiki.](github.com/mindaptiv/ellen/wiki)  

##Contact  
josh@mindaptiv.com
