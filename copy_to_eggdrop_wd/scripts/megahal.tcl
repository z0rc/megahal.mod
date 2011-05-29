# This is an accompanying TCL script to go with the MegaHAL eggdrop module
# It is optional but strongly recommended

# Should the bot learn what is said to it or only converse? Set to on/off.
#set megahal_directory_resources "megahal.data/default"
#set megahal_directory_cache "megahal.data/default/brains"

# Should the bot learn what is said to it or only converse? Set to on/off.
set learnmode on

# What is the max size the brain should grow to?
# Set to the max number of nodes not ram
set maxsize 100000

# Flood protection: how many max lines per how many seconds should it respond to?
set floodmega 10:60

# Which channels NOT to respond in when the botnick is mentioned (use space delimited list)
# Note: this includes the 'botnick: ' learning trigger
set respondexcludechans ""

# Which keywords besides the botnick should be responded to (use space delimited list)
# Examples can include alternative bot nicknames, people's names, special words like hello, music, etc
set responsekeywords ""

# How often should it talk in response to anything that is said in the channels?
# Set to # of lines or to 0 to turn this off.
set talkfreq 40

# Which channels NOT to talk in in response to all chatter (use space delimited list)
set talkexcludechans ""

# How often should it learn phrases that are said in the channels?
# Set to # of lines or to 0 to turn this off.
set learnfreq 2


# The following settings change the 'psychotic' level or personality of the AI slightly

# Max context size
# Valid values are 1-5
# 2 is highly recommended, 3 is much more boring but it will also produce much more coherent sentences
# 1 will make it babble incoherently a lot of the time and 4-5 will turn it into a parrot instead of a fun AI
setmaxcontext 2

# Surprise mode on or off (0/1)
# This changes the way it constructs sentences. 
# If on, it tries to find unconventional combinations of words which means much more fun but also more incoherent sentences
# If off, sentences are safer but more parrot-like so this is only recommended if the brain size is huge )in which case the bot has many safe options to use).
set surprise 1

# Max reply words
# This can help avoid long incoherent sentences that seem to run forever without making sense
# It limits the AI to create shorter sentences
# Recommended setting is about 25-40, set to 0 to allow unlimited size
set maxreplywords 30


#####################################################################


learningmode $learnmode

# bind the botnick
catch "unbind pub - hal: *pub:hal:"
bind pub - ${nick}: *pub:hal:
catch "unbind dcc - hal *dcc:hal"
bind dcc - $nick *dcc:hal
setmegabotnick $nick

# Save and trim the brain once every hour
bind time - "35 * * * *" auto_brainsave
proc auto_brainsave {min b c d e} { 
  global maxsize
  trimbrain $maxsize
  savebrain
}

bind pub m ".savebrain" pub_savebrain
proc pub_savebrain {nick uhost hand chan arg} {
 savebrain
 puthelp "PRIVMSG $chan :Brain saved"
}
bind pub n ".trimbrain" pub_trimbrain
proc pub_trimbrain {nick uhost hand chan arg} {
 global maxsize
 set arg1 [lindex $arg 0]
 if {$arg1 == "" || ![isnum $arg1]} {
	set arg1 $maxsize
 }
 trimbrain $arg1
 puthelp "PRIVMSG $chan :Brain trimmed"
}

bind pub n ".lobotomy" pub_lobotomy
proc pub_lobotomy {nick uhost hand chan arg} {
 savebrain
 file delete megahal.old
 file copy megahal.brn megahal.old
 file delete megahal.brn
 reloadbrain
 savebrain
 puthelp "PRIVMSG $chan :Lobotomy completed! Creating a new brain..." 
}

bind pub - ".braininfo" pub_braininfo
proc pub_braininfo {nick uhost hand chan arg} {
  global learnmode
  set for [treesize -1 0]
  set back [treesize -1 1]
  puthelp "PRIVMSG $chan :My current vocabulary consists of [lindex $for 0] words, my brain size is [expr [lindex $for 1]+[lindex $back 1]] nodes, and learning mode is $learnmode" 
#  if {[file exists megahal.old]} {
#    puthelp "PRIVMSG $chan :This brain has been growing for [duration [expr [unixtime] - [file mtime megahal.old]]]" 
#  }
}

bind pub n ".learningmode" pub_learningmode
proc pub_learningmode {nick uhost hand chan arg} {
  global learnmode
  set arg1 [lindex $arg 0]
  if {$arg1 == "" || ($arg1 != "on" && $arg1 != "off")} {
   puthelp "PRIVMSG $chan :Usage: .learningmode on/off"
   return
  }
  set learnmode $arg1
  learningmode $learnmode
  puthelp "PRIVMSG $chan :Learning mode is now set to $arg1" 
}

bind pub m ".talkfrequency" pub_talkfrequency
proc pub_talkfrequency {nick uhost hand chan arg} {
  global talkfreq
  set arg1 [lindex $arg 0]
  if {$arg1 == ""} {
   puthelp "PRIVMSG $chan :Talking frequency is set to $talkfreq lines" 
   return
  }
  set talkfreq $arg1
  puthelp "PRIVMSG $chan :Talking frequency is now set to $arg1 lines" 
}

bind pub n ".restorebrain" pub_restorebrain
proc pub_restorebrain {nick uhost hand chan arg} {
  if {[file exists megahal.old]} {
    file delete megahal.brn
    file copy megahal.old megahal.brn
    reloadbrain
    puthelp "PRIVMSG $chan :Old brain restored!"
  } else {
    puthelp "PRIVMSG $chan :Old brain not found!" 
  }
}

proc isnum {num} {
  for {set x 0} {$x < [string length $num]} {incr x} {
  if {[string trim [string index $num $x] 0123456789.] != ""} {return 0}
  }
 return 1
}
