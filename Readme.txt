MegaHAL module for eggdrop
Version 3.5
By Jason Hutchens and Zev ^Baron^ Toledano (megahal at thelastexit.net)

Artificially Intelligent conversation with learning capability and
psychotic personality.


+----------------------------------------------------------------------------+
|                                                                            |
| Copyright (C) 2009 Zev Toledano                                          |
| Copyright (C) 1999 Jason Hutchens                                          |
|                                                                            |
| This program is free software; you can redistribute it and/or modify it    |
| under the terms of the GNU General Public License as published by the Free |
| Software Foundation; either version 2 of the license or (at your option)   |
| any later version.                                                         |
|                                                                            |
| This program is distributed in the hope that it will be useful, but        |
| WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY |
| or FITNESS FOR A PARTICULAR PURPOSE.  See the Gnu Public License for more  |
| details.                                                                   |
|                                                                            |
| You should have received a copy of the GNU General Public License along    |
| with this program; if not, write to the Free Software Foundation, Inc.,    |
| 675 Mass Ave, Cambridge, MA 02139, USA.                                    |
|                                                                            |
+----------------------------------------------------------------------------+


Note: The original AI work and program was written by J. Hutchens in 1999.
This eggdrop version was coded by Zev Toledano and it necessitated
various major changes to the brain structure and code in order to keep the
brain at a manageable size and stop it from growing ad infinitum.
Numerous commands and fun features were added as well. In other words, this is
NOT merely a port but a different version of MegaHAL.

Important: Old brains (*.brn files) from the original MegaHAL program will NOT
work with this version! You must start a new one. For a list of version changes,
see the megahal.c file.

If this program formats your hard drive by mistake, or gets your dog pregnant,
keep your complaints to yourself. If you wish to thank me or send reasonable
suggestions and new ideas for improvement, send me an email at megahal at thelastexit.net
J. Hutchens is NO longer working on it since 1999 and it has changed a lot since.

This was tested on eggdrops versions 1.5.x and higher only.


-----------------------------

HOW TO GET IT RUNNING

1. Unzip or untar-gzip the package into the src/mod/ directory of your eggdrop.
2. Compile your eggdrop as usual from the main source directory.
3. Either 'make install' your eggdrop or copy the megahal.so file to your
   eggdrop/modules directory.
4. Copy the megahal.aux .trn .ban and .swp files to the main eggdrop directory
   together with the eggdrop binary.
5. Load the module either with "loadmodule megahal" or in the conf using
   ".loadmod megahal".
6. Copy the optional megahal.tcl script into your eggdrop/scripts directory and
   load it as well (in the conf: "source scripts/megahal.tcl").
If you don't use the tcl script, you must set the BOTNICK variables in megahal.c
before compiling.


-----------------------------

COMMANDS

Note that many of the following commands are added by the optional megahal.tcl script.
Feel free to bind more commands to DCC or public as necessary using the provided
TCL commands.


PUBLIC (in channels)
Any line with the botnick in it will get the bot to respond.
To teach it something use - <botnick>: bla bla
.braininfo - displays current brain information such as size, etc.
.megaver - displays version information

Bot masters only:
.forget <text> - this will try to forget the specified text by finding an
                 original phrase that matches the text as closely as possible.
.forgetword <word> - this will forget all phrases containing that word
.savebrain - saves the brain, dictionary and phrase files. Note that the brain
             is automatically saved once every hour and when the bot goes down.
.talkfrequency <#oflines> - sets how often the bot should respond to chatter in
                            public channels.

Bot owners only:
.trimbrain <#ofnodes> - trims the brain down to roughly the specified size.
.lobotomy - backups the current brain, deletes it and starts a new one.
.restorebrain - looks for megahal.old and restores it, deleting the current one.
.learningmode <on/off> - sets whether the bot should learn from <botnick>: etc
                         or only converse.


DCC
To teach it something (and to get a response) use - .<botnick> bla bla
.megaver - displays version information

Bot masters only:
.forget <text> - this will try to forget the specified text by finding an
                 original phrase that matches the text as closely as possible.


TCL
savebrain - duh
reloadbrain - take a guess
trimbrain <#ofnodes> - same as the public command
learningmode <on/off> - ditto
talkfrequency <#oflines> - ditto
setmegabotnick <botnick> - this sets the botnick to look out for in all public
                           chatter. The botnick bindings still have to be
                           changed separately though (see megahal.tcl).
setmaxcontext <size> - use this to change the context size of a brain
                       dynamically. Valid values are 1-5. 2 is highly
                       recommended, 3 is much more boring but it will also
                       produce much more coherent sentences. 1 will make it
                       babble incoherently a lot of the time and 4-5 will turn
                       it into a parrot instead of a fun AI.
reloadphrases - this will reload the brain from scratch but by relearning all
                the phrases in the megahal.phr file only. You can edit the phr
                file or restore an old one this way and weed out the brain.
learnfile <filename> - this will learn all the phrases it finds in the specified
                       file and add them to the current brain.


The following are only useful for people interested in sticking their fingers
into the brain. They are hacks, not user friendly services!

treesize <subbranch> <forwards/backwards> - displays four numbers that give info
                                            on the specified branch:
                                            1. # of branches
                                            2. # of all subbranches/nodes
                                            3. the count counter
                                            4. the usage counter

viewbranch <subbranch> <forwards/backwards> - shows the actual words contained
                                              in a branch and all its subranches
                                              together with the usage counter
                                              for each word.

 <subbranch> - set this to -1 for the main branch, any other number for a
               specified subbranch (it can only go one level down)
 <forwards/backwards> - 0 for the forward tree, 1 for backward tree


TCL VARIABLES
talkfreq - int - see talkfrequency command
learnfreq - int - how often to learn phrases said in the channels
maxsize - int - max brain size. see trimbrain command
maxreplywords - int - max size for replies. This can help keep those endless
                incoherent sentences under control
surprise - int - 0 for off, 1 for on. If on, the AI tries to generate more
           surprising replies.
talkexcludechans - string - space delimited list of chans to exclude from the
                   public chatter (talkfrequency).
respondexcludechans - string - space delimited list of chans to exclude from
                      the bot responses (all lines with the botnick in them)
responsekeywords - string - space delimited list of keywords besides the botnick to respond to
floodmega - couplet - flood settings (how many lines in how many seconds)





-----------------------------

The original source code and information can be found at http://www.amristar.com.au/~hutch/
For more information, see also the readme-old.txt file.

Zev Toledano
