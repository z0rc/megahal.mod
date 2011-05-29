/*===========================================================================*/
/*
 *  Copyright (C) 2009 Zev Toledano
 *  Original Copyright (C) 1998/1999 Jason Hutchens
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the license or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the Gnu Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */
/*===========================================================================*/


/*
 * MegaHAL module for eggdrop v3.7
 * By Zev ^Baron^ Toledano (megahal at thelastexit.net) and Jason Hutchens
 * Artificially Intelligent conversation with learning capability and
 * psychotic personality.
 *
 * Note: The original AI work and program was written by J. Hutchens in 1999.
 * The eggdrop port was coded by Zev Toledano and it necessitated
 * various major changes to the brain structure and code in order to keep the
 * brain at a manageable size. Various commands and fun features were added as
 * well.
 * Important: Old brains (1.x/2.x *.brn files) will NOT work with this version (3.0)! You
 * must start a new one.
 *
 * (all old comments were moved to the end of this file)
 *
 * Additions and changes by Nexor:
 *
 * Ver 3.7 Nov 2010
 * - Fixed memory allocation bug in mystrdup
 * - Megahal training resources and other read only files moved to separate directories: megahal_directory_resource
 * - Megahal brains and generated files moved to separate directories: megahal_directory_cache
 * - Removed compilation warnings with fread
 * - Rewrited change_personality and load_personality
 *
 * Additions and changes by z0rc:
 *
 * Ver 3.6: Nov 2009
 *  - Support of multibyte text encodings based on locale settings
 *
 * Additions and changes by ^Baron^:
 *
 * Ver 3.5: Jul 2009
 *  - Made .forget detect smaller words
 *  - Fixed bug with newer eggdrops where it was responding twice
 *  - Chat now ignores pub channel commands
 *  - Fixed some buffer overruns
 *  - Removed more compilation warnings

 * Ver 3.4: Apr 2006
 *  - Added .forgetword pub command - note that this deletes all phrases containing that word
 *  - Fixed a bug in trimdictionary that was messing up the index, thus making the find_words less functional every time the dictionary got trimmed
 *  - Fixed a bug where only irc client codes in the text made it crash
 *  - Improved speed of trimdictionary
 *  - Fixed bug loading phrases from the brain without a terminator, causing rare bugs when deleting phrases from small brains
 *  - Fixed bug with spaces at end of phrases
 *  - Fixed occasional crash when loading brain
 *  - Fixed bug where responsekeywords weren't removed from phrase before they were learned
 *
 * Ver 3.3: Nov 2003
 *  - Improved forget command fuzzy logic to find better matching phrases
 *  - Added learnfreq x setting to make bot learn every x amount of phrases said in channels
 *  - It now also learns phrases with the botnick in the beginning or end of the phrase
 *     i.e. no need for the botnick: trigger
 *  - Added responsekeywords setting to enable keywords that must be responded to
 *
 * Ver 3.22: May 2001
 *  - Cleaned up the code so that no warnings will be issued by the compiler
 *  - Fixed a bug that caused crashes in eggdrop 1.6.4 and fixed a memory leak
 *  - Fixed a crash problem when the botnick was mentioned without anything else on some bots
 *  - Fixed a bug where failed realloc complaints were being issued when using .forget
 *
 * Ver 3.21: Mar 2001
 *  - Improved the anti-repeat check so that it does fuzzy searches in the
 *    previous replies before responding.
 *  - Added mirc/pirc/etc code stripping!
 *
 * Ver 3.2: Mar 2001
 *  - Fixed bug in find_phrase for the forget command - it wasn't finding
 *    bigger phrases
 *  - Fixed make_keywords so that it also adds words that start with the
 *    special char31 character (nospace)
 *  - Added a check for reply repetitions so that it doesn't say the same thing
 *    in 5 consecutive replies.
 *  - Fixed bug that seems to have been there all along where the find_words
 *    function was wrongly used with all dictionaries and it treated index 0
 *    as an error! Replies should be much more appropriate now!
 *  - Added a maxreplywords setting to limit the output to smaller lines
 *  - Added a check for repeats in the replies so that it doesn't get stuck in
 *    a loop and output repetitive sentences.
 *  - Fixed treatment of hyphenated words
 *  - Fixed bug where last word was being replaced by a period if it was
 *    not preceded by a space.
 *  - Users text and AI output is now outputted to the channel log files as well.
 *  - Fixed bug in delete_all_phrases where multiples were found and index got
 *    moved down while deleting.
 *  - Removed copybrain function - use setmaxcontext or reloadphrases instead.
 *  - It now checks for spaces first before learning new phrases.
 *  - Added reloadphrases function for reloading the brain from the megahal.phr
 *    file.
 *  - Added learnfile function for adding new phrases to the brain from a file.
 *  - Added surprise setting for turning surprise mode on or off.
 *  - Added setmaxcontext function for changing the order value in the brain
 *    dynamically.
 *
 * Ver 3.11: Feb 2001
 *  - Added flood protection
 *  - Added tcl vars for various settings
 *  - Made each channel mantain its own counter for talkfrequency
 *  - Added strings for excluding specific channels from either the bot responses
 *    to its nickname or from the talkfrequency stuff
 *  - talkfrequency can now be set to 0 to turn it off
 *
 * Ver 3.1: Feb 2001
 *  - .forget command both in dcc and public. Searches for closest matches and
 *    deletes a phrase from the brain.
 *  - .megaver command in dcc and public for displaying the version information
 *  - actions are now detected in public as well.
 *  - copybrain tcl command for copying one model to another. It can be used to
 *    start a fresh brain with only valid phrases in case anything went wrong
 *    or to change the order setting for an existing brain.
 *  - setmegabotnick tcl command for setting the bot nickname dynamically. The
 *    bindings still have to be changed separately though - this is only for the
 *    detection of the bot nick in all public chatter.
 *  - expmem - now roughly estimates the mem used by this module. The report
 *    function (used by the .status command) now also displays more information.
 *
 * Ver 3.0: Feb 2001
 *  - Tricky bug in trimbrain required a major change in the brain structure
 *    again to keep track of phrases (a phrase field was added to the model and
 *    LOTS of code changed to work with this).
 *  - Added BIG trimdictionary function for trimming words that arent used anymore.
 *
 * Ver 2.0: Jan 2001
 *  - In order to be able to trim the brain, the format of the brain was changed
 *    (yes old ones will not work anymore). Every phrase now starts with a
 *    special <START> symbol so that we can find phrases more easily.
 *  - Added a complex trimbrain function to trim the brain down to a specified
 *    size.
 *  - Spaces were abolished from the brain thus saving at least 60% of the memory
 *    footprint! This meant:
 *      Detecting spaces and removing them from the text or simply skipping over
 *       them instead of including them as words.
 *      Every 'word' that is not preceded by a space is now saved
 *       with a preceding special character (#31). This character is used as a
 *       flag on whether to add a space or not.
 *      Making the 'order' variable smaller (2 or 3) so that the context isnt too
 *       big and some randomness creeps in.
 *  - Added realloc functions for resizing a dictionary or tree.
 *  - Added function definitions finally for all eggdrop stuff and moved all the
 *    defs to the header file.
 *  - Changed everything to static but it didnt seem to help the ram (?)
 *  - Cleaned up some code that stayed behind from the non-eggdrop version.
 *
 * Ver 1.6: Jan 2001
 *  - Improved the viewbranch tcl command to make it more robust and MUCH nicer
 *    on the eyes.
 *  - Added the learningmode TCL command for setting the mode on or off. When off,
 *    it can converse without making the brain grow.
 *  - Made the bot nickname more easily changeable by setting the preprocessor
 *    variable BOTNICK.
 *  - It now listens to all public chatter and detects when the bot nickname is
 *    typed anywhere in the text then uses only the REST of the text to generate a
 *    reply. It still only learns through BOTNICK: though.
 *  - talkfrequency - TCL and variable for replying at the specified frequency to
 *    whatever is said in the public channels regardless of the text.
 *
 * Ver 1.5: Dec 2000
 *  - Removed the useless private MSG commands and conversation.
 *  - Added the following TCL commands so that stuff can be automated or
 *    interacted with through TCL:
 *      - savebrain - for saving the brain to disk.
 *      - treesize - for viewing information on the model and branches like
 *        branch size, amount of nodes and usage counters.
 *      - reloadbrain - for reloading the brain from file. This can be used by
 *        scripts to start a new brain or to restore old ones for example.
 *      - viewbranch - just for curiosity or debugging. it will show a branch or
 *        subbranch with all its data and subbranches.
 */

/* change these to your bot name */
#define BOTNICK "Kuro-sama"
#define BOTNICK2 "Kuro-sama:"
#define BOTNICK3 "Kuro-sama,"

#define MAKING_MEGAHAL
#define MODULE_NAME "MegaHAL"
#define VER "3.7"
#define VER1 3
#define VER2 7
#define COOKIE "MegaHAL83"
#include <stdlib.h>
/* megahal preproc directives */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <locale.h>
#include <wctype.h>
#define __USE_UNIX98
#include <wchar.h>
#include "megahal.h"
/* End megahal preproc directives */
#include "../module.h"
#include "../irc.mod/irc.h"
#include "../server.mod/server.h"

#undef global

/* predefinitions for megahal*/

static int rnd(int);
static int order = 2;
static int timeout = 1;
static DICTIONARY *ban = NULL;
static DICTIONARY *aux = NULL;
static SWAP *swp = NULL;
static bool used_key;
static char directory_cache[513] = DIR_DEFAULT_CACHE;
static char directory_resources[513] = DIR_DEFAULT_RESOURCES;

/* predefinitions for eggdrop port */

static Function *global = NULL;
static Function *irc_funcs = NULL, *server_funcs = NULL;
static DICTIONARY *words = NULL;
static MODEL *model = NULL;
static int talkfrequency = 40;
static int learnfrequency = 40;
static bool learningmode = TRUE;
static wchar_t glob_str[15000];
static wchar_t glob_buffer[513];
static wchar_t mbotnick[32] = _T(BOTNICK);
static int maxlines = 10, maxtime = 60, curlines = 0, curtime = 0;
static char texcludechans[513] = "", rexcludechans[513] = "", responsekeywords[513] = "";
static int maxsize = 100000;
static int maxreplywords = 0;
static int surprise = 1;
static DICTIONARY *prev1, *prev2, *prev3, *prev4, *prev5;

static cmd_t mega_dcc[] =
{
  {BOTNICK, "", dcc_megahal, NULL},
  {"forget", "m", dcc_forget, NULL},
  {"megaver", "", dcc_megaver, NULL},
  {NULL, NULL, NULL, NULL}
};

static cmd_t mega_pub[] =
{
  {BOTNICK2, "", pub_megahal, NULL},
  {BOTNICK3, "", pub_megahal, NULL},
  {"!megaver", "", pub_megaver, NULL},
  {"!forget", "m", pub_forget, NULL},
  {"!forgetword", "m", pub_forgetword, NULL},
  {NULL, NULL, NULL, NULL}
};

static cmd_t mega_pubm[] =
{
  {"*", "", pub_megahal2, "mega"},
  {NULL, NULL, NULL, NULL}
};

static cmd_t mega_ctcp[] =
{
  {"ACTION", "", pub_action, "mega"},
  {NULL, NULL, NULL, NULL}
};

static tcl_cmds mytcls[] =
{
  {"savebrain", tcl_savebrain},
  {"reloadbrain", tcl_reloadbrain},
  {"trimbrain", tcl_trimbrain},
  {"treesize", tcl_treesize},
  {"learningmode", tcl_learningmode},
  {"talkfrequency", tcl_talkfrequency},
  {"viewbranch", tcl_viewbranch},
  {"setmaxcontext", tcl_setmaxcontext},
  {"setmegabotnick", tcl_setmegabotnick},
  {"reloadphrases", tcl_reloadphrases},
  {"learnfile", tcl_learnfile},
  {0, 0}
};

static tcl_ints my_tcl_ints[] =
{
  {"talkfreq", &talkfrequency, 0},
  {"learnfreq", &learnfrequency, 0},
  {"maxsize", &maxsize, 0},
  {"maxreplywords", &maxreplywords, 0},
  {"surprise", &surprise, 0},
  {0, 0, 0}
};

static tcl_strings my_tcl_strings[] =
{
  {"talkexcludechans", texcludechans, 512, 0},
  {"respondexcludechans", rexcludechans, 512, 0},
  {"responsekeywords", responsekeywords, 512, 0},
  {"megahal_directory_resources", directory_resources, 512, 0},
  {"megahal_directory_cache", directory_cache, 512, 0},
  {0, 0, 0, 0}
};

static tcl_coups my_tcl_coups[] =
{
  {"floodmega", &maxlines, &maxtime},
  {0, 0, 0}
};

static Function megahal_table[] =
{
  (Function) megahal_start,
  (Function) megahal_close,
  (Function) megahal_expmem,
  (Function) megahal_report,
};

/* end predefinitions */

static wchar_t *locale_to_wchar(char *str)
{
	wchar_t *ptr;
	size_t s;

	Context;
	/* first arg == NULL means 'calculate needed space' */
	s = mbstowcs(NULL, str, 0);

	/* a size of -1 is triggered by an error in encoding; never
	   happen in ISO-8859-* locales, but possible in UTF-8 */
	if(s == -1)
		return NULL;

	/* malloc the necessary space */
	if((ptr = (wchar_t *)nmalloc((s+1)*sizeof(wchar_t))) == NULL)
		return NULL;

	/* really do it */
	mbstowcs(ptr, str, s);

	/* ensure NULL-termination */
	ptr[s] = L'\0';

	/* remember to free() ptr when done */
	return ptr;
}

static char *wchar_to_locale(wchar_t *str)
{
	char *ptr;
	size_t s;

	Context;
	/* first arg == NULL means 'calculate needed space' */
	s = wcstombs(NULL, str, 0);

	/* a size of -1 means there are characters that could not
	   be converted to current locale */
	if(s == -1)
		return NULL;

	/* malloc the necessary space */
	if((ptr = (char *)nmalloc(s+1)) == NULL)
		return NULL;

	/* really do it */
	wcstombs(ptr, str, s);

	/* ensure NULL-termination */
	ptr[s] = '\0';

	/* remember to free() ptr when done */
	return ptr;
}

static wchar_t *mynewsplit(wchar_t **rest)
{
	register wchar_t *o, *r;

	Context;
	if(!rest)
		return *rest = L"";
	o = *rest;
	while(*o == L' ')
		o++;
	r = o;
	while(*o && (*o != L' '))
		o++;
	if(*o)
		*o++ = 0;
	*rest = o;
	return r;
}

// rough estimate here - if you want precise numbers do it yourself :-p
static int megahal_expmem()
{
	register int i;
	int size = 0, tmp;

	Context;
	size += sizeof(MODEL);
	tmp = recurse_tree(model->forward);
	size += tmp*sizeof(TREE);
	size += tmp*sizeof(TREE *);
	tmp = recurse_tree(model->backward);
	size += tmp*sizeof(TREE);
	size += tmp*sizeof(TREE *);

	for(i=0; i<model->phrasecount; i++)
		size += model->phrase[i][0]+1*sizeof(BYTE2);
	size += model->phrasecount*sizeof(BYTE2 *);

	size += dictionary_expmem(model->dictionary);
	size += dictionary_expmem(ban);
	size += dictionary_expmem(aux);
	size += dictionary_expmem(words);

	size += sizeof(SWAP);
	for(i=0; i<swp->size; i++) {
		size += swp->from[i].length*sizeof(wchar_t);
		size += swp->to[i].length*sizeof(wchar_t);
	}
	size += swp->size*sizeof(STRING)*2;

	return size;
}

static int dictionary_expmem(DICTIONARY *dictionary)
{
	register int i;
	int size = 0;

	Context;
	size += sizeof(DICTIONARY);
	for (i=0; i<dictionary->size; i++)
		size += dictionary->entry[i].length*sizeof(wchar_t);
	size += dictionary->size*sizeof(STRING);
	size += dictionary->size*sizeof(BYTE2);

	return size;
}


static wchar_t *megahal_close()
{
	p_tcl_bind_list H_temp;

	Context;
	save_model("megahal.brn", model);
	rem_builtins(H_dcc, mega_dcc);
	rem_builtins(H_pubm, mega_pubm);
	rem_builtins(H_ctcp, mega_ctcp);
	if((H_temp = find_bind_table("pub")))
		rem_builtins(H_temp, mega_pub);
	rem_tcl_ints(my_tcl_ints);
	rem_tcl_coups(my_tcl_coups);
	rem_tcl_strings(my_tcl_strings);
	rem_tcl_commands(mytcls);
	module_undepend(MODULE_NAME);
	free_model(model);
	free_words(ban);
	free_dictionary(ban);
	free_words(aux);
	free_dictionary(aux);
	free_swap(swp);
	free_words(words);
	free_dictionary(words);
	free_words(prev1);
	free_dictionary(prev1);
	free_words(prev2);
	free_dictionary(prev2);
	free_words(prev3);
	free_dictionary(prev3);
	free_words(prev4);
	free_dictionary(prev4);
	free_words(prev5);
	free_dictionary(prev5);
	return NULL;
}

char *megahal_start(Function * global_funcs)
{
	p_tcl_bind_list H_temp;
	global = global_funcs;

	Context;
	module_register(MODULE_NAME, megahal_table, VER1, VER2);
	if(!(irc_funcs = module_depend(MODULE_NAME, "irc", 1, 0)))
		return "You need the irc module to use the megahal module.";
	if(!(server_funcs = module_depend(MODULE_NAME, "server", 1, 0)))
		return "You need the server module to use the megahal module.";
	add_builtins(H_dcc, mega_dcc);
	add_builtins(H_pubm, mega_pubm);
	add_builtins(H_ctcp, mega_ctcp);
	add_tcl_ints(my_tcl_ints);
	add_tcl_coups(my_tcl_coups);
	add_tcl_strings(my_tcl_strings);
	add_tcl_commands(mytcls);
	if((H_temp = find_bind_table("pub")))
		add_builtins(H_temp, mega_pub);
	words=new_dictionary();
	prev1=new_dictionary();
	prev2=new_dictionary();
	prev3=new_dictionary();
	prev4=new_dictionary();
	prev5=new_dictionary();
	/*
	 *	Load the default personality.
	 */
	change_personality(&model, NULL, NULL);
	putlog(LOG_MISC, "*", "MegaHAL v%s by z0rc loaded.", VER);

	return NULL;
}

/* a report on the module status */
static void megahal_report(int idx, int details)
{
	Context;
	if(details) {
		dprintf(idx, "     by z0rc, Zev ^Baron^ Toledano and Jason Hutchens\n");
		dprintf(idx, "     words: %d, nodes: %d\n", model->forward->branch, (recurse_tree(model->backward) + recurse_tree(model->forward)));
		dprintf(idx, "     using %d bytes\n", megahal_expmem());
	}
}

// Next, a hacked attempt at strdup(), since I can't use malloc() anywhere.
static wchar_t *mystrdup(const wchar_t *s)
{
	wchar_t *mytmp = nmalloc((wcslen(s)+1)*sizeof(wchar_t));

	Context;
	if(mytmp == NULL)
		return NULL;
	else wcscpy(mytmp, s);

	return mytmp;
}

static void mystrlwr(wchar_t *string)
{
	size_t i;
	Context;
	for(i=0; i<wcslen(string); ++i)
		string[i]=(wchar_t)towlower(string[i]);
}

// find pointer to sub inside s
static const wchar_t *mystrstr(const wchar_t *s, const wchar_t *sub)
{
	Context;
	if (!*sub)
		return s;
	for(; *s; ++s) {
		if(*s == *sub) {
			/*
			*	Matched starting char -- loop through remaining chars.
			*/
			const wchar_t *h, *n;
			for(h = s, n = sub; *h && *n; ++h, ++n) {
				if (*h != *n)
					break;
			}
			if(!*n) /* matched all of 'sub' to null termination */
				return s; /* return the start of the match */
		}
	}
	return NULL;
}

static bool floodcheck()
{
	Context;
	if(!maxlines || !maxtime)
		return TRUE;
	if((now - curtime) > maxtime) {
		curtime = now;
		curlines = 0;
	}
	curlines++;
	if(curlines > maxlines)
		return FALSE;
	return TRUE;
}


static char *istextinlist(char *text, char *list)
{
	wchar_t *wtext, *wlist;
	wchar_t *ch, *pbuf;

	Context;
	setlocale(LC_ALL, "");
	wtext = locale_to_wchar(text);
	wlist = locale_to_wchar(list);
	wchar_t buf[wcslen(wlist)+1];
	wcscpy(buf, wlist);
	wcsncpy(glob_buffer, wtext, 512);
	glob_buffer[512] = L'\0';
	mystrlwr(buf);
	mystrlwr(glob_buffer);

	nfree(wtext);
	nfree(wlist);
	pbuf = buf;
	while(wcslen(pbuf) > 0)
	{
		ch = mynewsplit(&pbuf);
		if(!wcscmp(glob_buffer, ch))
			return wchar_to_locale(glob_buffer);
	}
	return NULL;
}

static char *istextinlist2(STRING text, char *list)
{
	wchar_t *ch, *pbuf, *wlist;

	Context;
	setlocale(LC_ALL, "");
	wlist = locale_to_wchar(list);
	wchar_t buf[wcslen(wlist)+1];
	wcscpy(buf, wlist);
	wcsncpy(glob_buffer, text.word, text.length);
	glob_buffer[text.length] = L'\0'; // length = byte
	mystrlwr(buf);
	mystrlwr(glob_buffer);

	nfree(wlist);
	pbuf = buf;
	while(wcslen(pbuf) > 0)
	{
		ch = mynewsplit(&pbuf);
		if(!wcscmp(glob_buffer, ch))
			 return wchar_to_locale(glob_buffer);
	}
	return NULL;
}

static int countchans()
{
	struct chanset_t *ch;
	int c = 0;

	Context;
	for(ch = chanset; ch; ch = ch->next)
		c++;

	return c;
}

static int getchannum(char *chan)
{
	struct chanset_t *ch;
	int c = 0;

	Context;
	for(ch = chanset; ch; ch = ch->next)
	{
		if(!strcmp(chan, ch->dname))
			break;
		c++;
	}

	return c;
}

static void do_megahal(int idx, char *prefix, char *text, bool learnit, char *nick, char *chan)
{
	char stuff[strlen(prefix) + 50], *lhalreply, *lmbotnick;
	wchar_t *halreply, *wtext;

	Context;
	/* Is there anything to parse? */
	if(!text[0]) {
		sprintf(stuff, "%sSo tell me something already.\n", prefix);
		dprintf(idx, stuff);
		return;
	}

	setlocale(LC_ALL, "");
	wtext = locale_to_wchar(text);
	upper(wtext);
	if(mystrstr(wtext, L"HTTP") != NULL) {
		nfree(wtext);
		return;
	}
	make_words(wtext, words);
	Context;
	if(learningmode && learnit)
		learn(model, words);
	halreply = generate_reply(model, words);
	Context;
	capitalize(halreply);
	lhalreply = wchar_to_locale(halreply);
	dprintf(idx, "%s%s\n", prefix, lhalreply);
	lmbotnick = wchar_to_locale(mbotnick);
	if(nick)
		putlog(LOG_PUBLIC, chan, "<%s> %s, %s", lmbotnick, nick, lhalreply);
	else if(chan)
		putlog(LOG_PUBLIC, chan, "<%s> %s", lmbotnick, lhalreply);
	nfree(lmbotnick);
	nfree(lhalreply);
	nfree(wtext);
}

static int pub_megahal(char *nick, char *host, char *hand, char *channel, char *text)
{
	char prefix[strlen(channel) + strlen(nick) + 13];
	struct chanset_t *chan = findchan(channel);

	Context;
	if(!floodcheck())
		return 0;
	if(istextinlist(channel, rexcludechans))
		return 0;
	if(chan != NULL) {
		sprintf(prefix, "PRIVMSG %s :%s: ", channel, nick);
		char *lmbotnick=wchar_to_locale(mbotnick);
		putlog(LOG_PUBLIC, channel, "<%s> %s: %s", nick, lmbotnick, text);
		nfree(lmbotnick);
		do_megahal(DP_HELP, prefix, text, TRUE, nick, channel);
	}
	return 0;
}

static int pub_action(char *nick, char *host, char *hand, char *channel, char *key, char *text)
{
	pub_megahal2(nick, host, hand, channel, text);
	return 0;
}

static int pub_megahal2(char *nick, char *host, char *hand, char *channel, char *text)
{
	// counters per channel for chatter/learning
	static int *count = NULL, *learncount = NULL;
	static int chancount = 0, learnchancount = 0;

	char prefix[strlen(channel) + strlen(nick) + 13], *keyword = NULL;
	wchar_t buffer[512] = L"";
	wchar_t *wtext;

	int i, pos;
	struct chanset_t *chan = findchan(channel);
	bool learnit = FALSE, flg = TRUE;

	Context;
	setlocale(LC_ALL, "");
	wtext = locale_to_wchar(text);
	wcscpy(buffer,wtext);
	nfree(wtext);
	mystrlwr(buffer);
	if(mystrstr(buffer, L"http") != NULL) {
		return 0;
	}

	// handle and respond to phrases with the botnick or keywords in it immediately and then exit
	make_words(buffer, words); // dont make upper or it will mess things like mystrstr
	if(words->size == 0) { // might have stripped all codes and returned empty
		return 0;
	}
	if(words->size > 1) {
		if((wordcmp2(words->entry[0], mbotnick)==0) && ((words->entry[1].word[1] == L':') || (words->entry[1].word[1] == L','))) { // let this be handled by pub_megahal otherwise we respond twice
			return 0;
		}
	}
	if((words->entry[0].word[0] == L'.') || (words->entry[0].word[0] == L'!')) { // pub command - ignore
		return 0;
	}
	Context;
	flg = (istextinlist(channel, rexcludechans)!=NULL); // dont respond/chat in channels that are excluded but learnfrequency is still enabled for now...
	for (i=0; i<words->size; i++) // find whether one of the response keywords is in the text - find first only
		if((keyword = istextinlist2(words->entry[i], responsekeywords)))
			break;
	if(chan != NULL && (mystrstr(buffer, mbotnick) || keyword) && !flg) { // either botnick or keyword said - respond immediately and exit
		Context;
		if(!floodcheck()) {
			return 0;
		}
		if(!keyword)
			keyword = wchar_to_locale(mbotnick);
		// if keyword used in beginning or end of phrase, learn it
		if(wordcmp2(words->entry[0], locale_to_wchar(keyword))==0 || wordcmp2(words->entry[(words->size)-2], locale_to_wchar(keyword))==0) { // -2 to exclude the period
			learnit = TRUE;
			sprintf(prefix, "PRIVMSG %s :%s: ", channel, nick);
		} else {
			sprintf(prefix, "PRIVMSG %s :%s, ", channel, nick);
		}
		// remove the botnick from the text
		if(mystrstr(buffer, locale_to_wchar(keyword))) {
			pos = (int)(mystrstr(buffer, locale_to_wchar(keyword)) - buffer);
			for(i=pos; i<(wcslen(buffer)-wcslen(locale_to_wchar(keyword))); i++)
				buffer[i] = buffer[i+wcslen(locale_to_wchar(keyword))];
			buffer[i]=L'\0';
		}
		char *lbuffer=wchar_to_locale(buffer);
		do_megahal(DP_HELP, prefix, lbuffer, learnit, nick, channel);
		nfree(lbuffer);
		return 0;
	}

	// Learn this phrase?
	if(learningmode && learnfrequency>0) {
		// initialize the count array - one int per chan (first time only)
		Context;
		if(countchans() && learncount == NULL) {
			learncount = (int *)nmalloc(sizeof(int)*countchans());
			for (i=0; i<countchans(); i++)
				learncount[i] = 0;
			learnchancount = countchans();
		}
		// if number of chans changed, realloc the array
		if(countchans() && learnchancount != countchans()) {
			learncount = (int *)nrealloc((int *)(learncount), sizeof(int)*countchans());
			learnchancount = countchans();
		}

		// increment appropriate counter
		if(learncount[getchannum(channel)] < learnfrequency) {
			learncount[getchannum(channel)]++;
		} else {
			upper(buffer);
			make_words(buffer, words);
			if(words->size > (model->order)) { // only learn phrases with minimum amount of words
				learn(model, words);
				learncount[getchannum(channel)] = 0;
			}
		}
	}

	// From here, chatter stuff only
	// check if chatter is turned off or if this chan is in the exclude list
	Context;
	if(istextinlist(channel, texcludechans) || talkfrequency == 0)
		return 0;

	// initialize the count array - one int per chan (first time only)
	if(countchans() && count == NULL) {
		count = (int *)nmalloc(sizeof(int)*countchans());
		for (i=0; i<countchans(); i++)
			count[i] = 0;
		chancount = countchans();
	}
	// if number of chans changed, realloc the array
	if(countchans() && chancount != countchans()) {
		count = (int *)nrealloc((int *)(count), sizeof(int)*countchans());
		chancount = countchans();
	}

	// increment appropriate counter
	if(count[getchannum(channel)] < talkfrequency) {
		count[getchannum(channel)]++;
		return 0;
	} else {
		if (!floodcheck())
			return 0;
		count[getchannum(channel)] = 0;
	}

	if(chan != NULL) {
		sprintf(prefix, "PRIVMSG %s :", channel);
		do_megahal(DP_HELP, prefix, text, FALSE, NULL, channel);
	}
	if(keyword)
		nfree(keyword);
	return 0;
}

static int dcc_megahal(struct userrec *u, int idx, char *par)
{
	Context;
	if(!floodcheck())
		return 0;
	do_megahal(idx, "", par, TRUE, NULL, NULL);
	return 0;
}



static int dcc_forget(struct userrec *u, int idx, char *text)
{
	register int j;
	int phrase;
	bool fnd;
	DICTIONARY *words;
	wchar_t *output, *wtext;

	Context;
	setlocale(LC_ALL, "");
	if(!text[0])
		return 0;

	words = new_dictionary();
	wtext = locale_to_wchar(text);
	phrase = find_phrase(wtext, &fnd);
	if(fnd) {
		words->size=model->phrase[phrase][0]-1;
		if(realloc_dictionary(words)==NULL) {
			error("dcc_forget", "Unable to reallocate dictionary");
			return 0;
		}
		for(j=0; j<words->size; j++)
			words->entry[j] = model->dictionary->entry[model->phrase[phrase][j+1]];

		output = make_output(words);
		capitalize(output);
		char *loutput=wchar_to_locale(output);
		dprintf(idx, "You mean \"%s\"? OK, I'll try...\n", loutput);
		nfree(loutput);
		del_all_phrases(phrase);
		trimdictionary();
	} else {
		dprintf(idx, "There is no way that I am going to forget about that, sorry.\n");
	}

	nfree(wtext);
	return 0;
}

static int pub_forget(char *nick, char *host, char *hand, char *channel, char *text)
{
	register int j;
	int phrase;
	bool fnd;
	DICTIONARY *words;
	wchar_t *output, *wtext;

	Context;
	setlocale(LC_ALL, "");
	putlog(LOG_MISC, "*", "forget  %s  by %s", text, hand);
	if(!text[0])
		return 0;

	words = new_dictionary();
	wtext = locale_to_wchar(text);
	phrase = find_phrase(wtext, &fnd);
	if(fnd) {
		words->size=model->phrase[phrase][0]-1;
		if(realloc_dictionary(words)==NULL) {
			error("pub_forget", "Unable to reallocate dictionary");
			return 0;
		}
		for(j=0; j<words->size; j++)
			words->entry[j] = model->dictionary->entry[model->phrase[phrase][j+1]];

		output = make_output(words);
		capitalize(output);
		char *loutput=wchar_to_locale(output);
		dprintf(DP_HELP, "PRIVMSG %s :You mean \"%s\"? OK, I'll try...\n", channel, loutput);
		nfree(loutput);
		del_all_phrases(phrase);
		trimdictionary();
	} else {
		dprintf(DP_HELP, "PRIVMSG %s :There is no way that I am going to forget about that, sorry.\n", channel);
	}

	nfree(wtext);
	return 0;
}

// finds the closest matching phrase in the model to some text if (possible)
static int find_phrase(wchar_t *text, bool *found)
{
	register int i, j, k;
	int maxSize = 500;
	BYTE2 symbols[maxSize];
	BYTE2 symbol;
	int size = 0, highmatch = 0, count = 0, phrase = 0;
	bool flag = TRUE;

	Context;
	upper(text);
	make_words(text, words);
	if(words->size == 0)
		return 0;

	// create an array of unique symbols
	// make sure the words exist in the main dictionary already and that they arent repeated
	// we compress repeated symbols for two reasons: 1. so that we dont find repeated matches later
	// 2. in case some parts of the phrase were repeated by the bot and the user tries to make it forget that

	for(i=0; i<words->size; i++) {
		if(!(symbol = find_word(model->dictionary, words->entry[i])))
			continue;
		// minimum 2 letters in each word
		if(model->dictionary->entry[symbol].length < 2)
			continue;
		flag = TRUE;
		for(j=0; j<size; j++)
			if(symbols[j] == symbol)
				flag = FALSE;
		if(size >= maxSize)
			flag = FALSE;
		if(flag)
			symbols[size++] = symbol;
	}

	// now we try to find the closest match in our existing phrases
	for(i=0; i<model->phrasecount; i++) {

		// check that its at least a third of the size of the phrase or else even tiny phrases can match many repeated symbols in a long one
		if(size < ((model->phrase[i][0]-1)/3))
			continue;

		count = 0;
		for(j=1; j<model->phrase[i][0]-1; j++)
			for(k=0; k<size; k++)
				if(symbols[k] == model->phrase[i][j])
					count++;
		// check minimum length
		if(count < model->order)
			continue;
		// check that it matches at least a third of the phrase
		if(count < ((model->phrase[i][0]-1)/3))
			continue;

		// compare to previous matches
		if(count > highmatch) {
			highmatch = count;
			phrase = i;
		}
	}

	if(highmatch == 0)  {
		*found = FALSE;
		return 0;
	} else {
		*found = TRUE;
		return phrase;
	}
}

static int pub_forgetword(char *nick, char *host, char *hand, char *channel, char *text)
{
	register int i, j;
	BYTE2 symbol;
	int num=0;
	bool flag;
	DICTIONARY *words=NULL;
	wchar_t *wtext=NULL;

	if(!text[0])
		return 0;

	Context;
	setlocale(LC_ALL, "");
	wtext=locale_to_wchar(text);
	putlog(LOG_MISC, "*", "forget %s by %s", text, hand);
	words=new_dictionary();
	upper(wtext);
	make_words(wtext, words);
	if(!(symbol = find_word(model->dictionary, words->entry[0]))) {
		dprintf(DP_HELP, "PRIVMSG %s :I am not familiar with that word.\n", channel);
		nfree(wtext);
		return 0;
	}

	for(i=model->phrasecount-1; i>=0; i--) {
		flag = FALSE;
		for(j=1; j<=model->phrase[i][0]; j++)
			if(model->phrase[i][j] == symbol)
				flag = TRUE;
		if(flag) {
			del_phrase(i);
			num++;
		}
	}
	trimdictionary();
	capitalize(wtext);
	char *lwtext=wchar_to_locale(wtext);
	dprintf(DP_HELP, "PRIVMSG %s :%s has been mentioned to me %d times in the past. But it's all forgotten now.\n", channel, lwtext, num);
	nfree(lwtext);
	free_dictionary(words);
	nfree(wtext);
	return 0;
}

// deletes all phrases that are identical to the specified one (phrases can be entered twice, upping the counters)
static void del_all_phrases(int phrase)
{
	register int i, j;
	bool flag;

	Context;
	for(i=model->phrasecount-1; i>=0; i--) {
		if(model->phrase[i][0] != model->phrase[phrase][0])
			continue;
		if(i == phrase)
			continue;
		flag = TRUE;
		for(j=1; j<=model->phrase[i][0]; j++)
			if(model->phrase[i][j] != model->phrase[phrase][j])
				flag = FALSE;
		if(flag) {
			del_phrase(i);
			if(i<phrase)
				phrase--;
		}
	}
	del_phrase(phrase);
}


static int dcc_megaver(struct userrec *u, int idx, char *text)
{
	Context;
	if(!floodcheck())
		return 0;
	dprintf(idx, "MegaHAL module v%s by z0rc, Zev ^Baron^ Toledano and Jason Hutchens\n", VER);
	return 0;
}

static int pub_megaver(char *nick, char *host, char *hand, char *channel, char *text)
{
	Context;
	if(!floodcheck())
		return 0;
	dprintf(DP_HELP, "PRIVMSG %s :MegaHAL module v%s by z0rc, Zev ^Baron^ Toledano and Jason Hutchens\n", channel, VER);
	return 0;
}


// returns the amount of nodes/leaves in a tree by recursing through all its branches
static int recurse_tree(TREE *node)
{
	int size = 0;
	register int i;

	Context;
	size++;
	for(i=0; i<node->branch; ++i)
		size += recurse_tree(node->tree[i]);

	return size;
}


// returns the amount of branches and nodes in a tree
// if branch is specified it goes to a subbranch of the main 2 TREES
// forward and backward can be specified too
static int tcl_treesize STDVAR
{
	char s[50];
	int branch = -1;
	int backward = 0;

	Context;
	if(argv[1])
		branch = atoi(argv[1]);
	if(argv[2])
		backward = atoi(argv[2]);

	if(backward) {
		if((branch < model->backward->branch) && (branch > -1))
			sprintf(s, "%d %d %d %d", model->backward->tree[branch]->branch, recurse_tree(model->backward->tree[branch]), model->backward->tree[branch]->count, model->backward->tree[branch]->usage);
		else
			sprintf(s, "%d %d %d %d", model->backward->branch, recurse_tree(model->backward), model->backward->count, model->backward->usage);
	} else {
		if((branch < model->forward->branch) && (branch > -1))
			sprintf(s, "%d %d %d %d", model->forward->tree[branch]->branch, recurse_tree(model->forward->tree[branch]), model->forward->tree[branch]->count, model->forward->tree[branch]->usage);
		else
			sprintf(s, "%d %d %d %d", model->forward->branch, recurse_tree(model->forward), model->forward->count, model->forward->usage);
	}

	Tcl_AppendResult(irp, s, NULL);
	return TCL_OK;
}


/* The following two functions are just for playing around in the brain and to view branches
   They're a tad messy but can be used to learn about the brain, debug, etc */

static void recurse_branch(TREE *node)
{
	static int level=0;
	register int i, j, k;
	int tmp=0;
	static wchar_t s[10], ss[50] = L"";
	static bool newl = TRUE;
	static int length[3];

	Context;
	if(model->dictionary && (node->symbol < model->dictionary->size)) {
		if(glob_str[wcslen(glob_str)-1] == L'\n')
			wcscat(glob_str, ss);
		wcscat(glob_str, L" ");
		swprintf(s, 512, L"[%d]", node->symbol);
		wcscat(glob_str, s);
		if((int)model->dictionary->entry[node->symbol].word[0] == 31)
			tmp = 1;
		else
			tmp = 0;
		wcsncat(glob_str, model->dictionary->entry[node->symbol].word+tmp, model->dictionary->entry[node->symbol].length-tmp);
		swprintf(s, 512, L"(%ld)", node->usage);
		wcscat(glob_str, s);
		length[level] = model->dictionary->entry[node->symbol].length+1+wcslen(s);
	}

	for(i=0; i<node->branch; ++i) {
		++level;
		newl = TRUE;
		recurse_branch(node->tree[i]);
		--level;
		if(newl)
			wcscat(glob_str, L"\n");
		ss[0] = L'\0';
		for (j=0; j<=level; j++)
			for (k=0; k<length[j]; k++)
				wcscat(ss, L" ");
		newl = FALSE;
	}
}

static int tcl_viewbranch STDVAR
{
	int branch = -1;
	int backward = 0;

	Context;
	setlocale(LC_ALL, "");
	if(argv[1])
		branch = atoi(argv[1]);
	if(argv[2])
		backward = atoi(argv[2]);
	glob_str[0]=L'\0';

	if(backward) {
		if ((branch > -1) && (branch < model->backward->branch)) {
			if(model->backward->tree[branch]->branch > 200)
				swprintf(glob_str, 512, L"Branch is too big");
			else
				recurse_branch(model->backward->tree[branch]);
		} else {
			if(model->backward->branch > 200)
				swprintf(glob_str, 512, L"Branch out of range or too big");
			else
				recurse_branch(model->backward);
		}
	} else {
		if ((branch > -1) && (branch < model->forward->branch)) {
			if(model->forward->tree[branch]->branch > 200)
				swprintf(glob_str, 512, L"Branch is too big");
			else
				recurse_branch(model->forward->tree[branch]);
		} else {
			if(model->forward->branch > 200)
				swprintf(glob_str, 512, L"Branch out of range or too big");
			else
				recurse_branch(model->forward);
		}
	}

	char *lglob = wchar_to_locale(glob_str);
	Tcl_AppendResult(irp, lglob, NULL);
	nfree(lglob);
	return TCL_OK;
}


// reloads and relearns the brain with a new order size
static int tcl_setmaxcontext STDVAR
{
	int neworder;

	Context;
	neworder = order;
	if(argv[1])
		neworder = atoi(argv[1]);
	if(neworder < 1 || neworder > 5 || neworder == order)
		return 0;

	order=neworder;
	reloadphrases();
	putlog(LOG_MISC, "*", "Brain transferred (order: %d)", order);
	return TCL_OK;
}

static int tcl_reloadphrases STDVAR
{
	Context;
	reloadphrases();
	putlog(LOG_MISC, "*", "Phrases reloaded");
	return TCL_OK;
}

static void reloadphrases()
{
  char filename[512];
	Context;

	free_model(model);
	model = new_model(order);

	snprintf(filename, sizeof(filename), "%s%smegahal.phr", directory_cache, SEP);
	train(model, filename);

}

static int tcl_learnfile STDVAR
{
	char filename[512];

	Context;
	BADARGS(2, 2, " <filename>");

	snprintf(filename, sizeof(filename), "%s%s%s", directory_resources, SEP, argv[1]);
	train(model, filename);

	putlog(LOG_MISC, "*", "Learned file: %s", filename);
	return TCL_OK;
}


/* Note: These are personal notes on the various ways I tried to trim the brain (^Baron^). This will not be of interest to many.
 *
 * To trim a brain one cannot just trim branches at random or older branches because:
 * 1. Branches are usually snippets of phrases and you will be left with nonsensical pieces in the end - but this is not as important as:
 * 2. You could delete all possible endings of a phrase and so the AI gets stuck in a loop trying to find the end! This is pretty straightforward but the next is very tricky:
 * 3. The way the halcontext is used, it is filled up with the other branches that are actually continuations of the
 *    current branch (if any) under another 'context' and since it usually has to use the longest context in order to generate a normal looking reply, it will get
 *    stuck since it can't find the next nearest longest context that will end or continue the sentence (study the reply&babble functions to understand this).
 *    I tried forcing it to use smaller contexts but that just caused tons of gibberish.
 *
 * I also tried trimming by changing the format of the brain so that sentences are always stored with a <BEGIN> marker as the first word in every sentence.
 * This way I knew how to find FULL sentences at least by going to the branch that starts with <BEGIN> and going on from there.
 * Then I used full contexts searches to hop from one branch to the next in order to create a real phrase just as if we were creating a reply to the user.
 * Some branches are hopped onto by random and therefore the sentence will often not be the same as the original that was learned by megahal,
 * but at least we dont leave behind scraps of sentences or incomplete contexts.
 *
 * This wasn't enough however because of a very tricky case: There are sentences that repeat themselves in the middle and therefore we may
 * branch off to the latter part of the sentence while trying to create a sentence and then leave the middle snippets behind and cause other
 * unexpected bugs and problems. For example, an even worse case scenario is that we will REPEAT the same snippet twice because we find
 * ourselves back in the same context a few times. This will even cause null pointer exceptions, etc.
 *
 * So in the end I changed the brain again so that it keeps track of phrases as soon as it learns them and stores them together with the model.
 * We can't go wrong like this if we delete the EXACT original sentence.
 */

static int tcl_trimbrain STDVAR
{
	int newsize;
	register int i;

	Context;
	putlog(LOG_MISC, "*", "Trimming brain...");
	newsize = maxsize;
	// get the size we are aiming for
	if(argv[1])
		newsize = atoi(argv[1]);

	while(newsize < (recurse_tree(model->backward) + recurse_tree(model->forward)) && (model->phrasecount > 0)) {
		// do 25 a time so we dont have too many loops and size checks going on
		for (i=0; i<25; i++) {
			if(model->phrasecount < 1)
				break;
			del_phrase(0);
		}
	}

	trimdictionary();
	putlog(LOG_MISC, "*", "Brain trimmed");
	return TCL_OK;
}


// deletes a phrase from the model using all its contexts, decrementing counters or deleting branches where necessary
static void del_phrase(int phrase)
{
	bool fnd = FALSE;
	register int j, k;
	BYTE2 size;

	Context;
	if (phrase >= model->phrasecount) return;
	size = model->phrase[phrase][0];

	// go through the words/symbols and start trimming sets of context branches one symbol at a time
	for (j=1; j<=size; j++) {
		initialize_context(model);

		// build the context using the next batch of symbols
		model->halcontext[0] = model->forward->tree[search_node(model->forward, model->phrase[phrase][j], &fnd)];
		for (k=1; k<=model->order; k++) {
			if ((k+j) > size)
				break;
			model->halcontext[k] = model->halcontext[k-1]->tree[search_node(model->halcontext[k-1], model->phrase[phrase][j+k], &fnd)];
		}

		// now decrement the usage and delete unused branches - must go backwards in case branches are erased
		for (k=model->order; k>0; k--)
			decrement_tree(model->halcontext[k], model->halcontext[k-1]);
		decrement_tree(model->halcontext[0], model->forward);
	}


	// start the trimming in the backwards tree
	// first move the <FIN> symbol to the other side because we are going backwards now
	for (j=size; j>1; j--)
		model->phrase[phrase][j] = model->phrase[phrase][j-1];
	model->phrase[phrase][1] = 1;

	for (j=size; j>0; j--) {
		initialize_context(model);

		model->halcontext[0] = model->backward->tree[search_node(model->backward, model->phrase[phrase][j], &fnd)];
		for (k=1; k<=model->order; k++) {
			if (k>j-1) break;
			model->halcontext[k] = model->halcontext[k-1]->tree[search_node(model->halcontext[k-1], model->phrase[phrase][j-k], &fnd)];
		}

		for (k=model->order; k>0; k--)
			decrement_tree(model->halcontext[k], model->halcontext[k-1]);
		decrement_tree(model->halcontext[0], model->backward);
	}

/*	// move the symbol back again
	for (j=1; j<size; j++)
		model->phrase[phrase][j] = model->phrase[phrase][j+1];
	model->phrase[phrase][size] = 1;
*/
	// remove the phrase from the model
	nfree(model->phrase[phrase]);
	for(j=phrase; j<model->phrasecount; j++)
		model->phrase[j] = model->phrase[j+1];
	model->phrasecount--;
	if(realloc_phrase(model) == NULL) {
		error("del_phrase", "Unable to reallocate phrase");
		return;
	}

}

// this decrements the usage and count counters in a branch and deletes branches that arent used anymore
static void decrement_tree(TREE *node, TREE *parent)
{
	register int i;
	int position;
	bool fnd;

	Context;
	if(node == NULL || parent == NULL)
		return;

	position = search_node(parent, node->symbol, &fnd);
	--parent->usage;
	if(--node->count < 1) {
		free_tree(node);
		for(i=position; i<parent->branch-1; i++)
			parent->tree[i] = parent->tree[i+1];
		--parent->branch;
		realloc_tree(parent);
	}
}

// tries to find words in the main dictionary that arent being used in the model anymore and deletes them and updates everything thats necessary
static void trimdictionary()
{
	register int i, j, k;
	int tmp = 0, tmp2 = 0, smallest = model->dictionary->size;
	int *syms = NULL;

	Context;
	/* First find words that arent being used in the dictionary, mark them with NULL
	   but dont remove them yet because the symbols will shift down and we wont be able to search properly for the rest!
	   Also mark the references in the dictionary index to these words with a unique high number but dont remove yet because the order is different there
	   we need a separate loop to remove them */
	syms = (int *)nmalloc(sizeof(int)*1);
	for(i=0; i<model->dictionary->size; ++i) { // must iterate over index
		if (model->dictionary->index[i]==0 || model->dictionary->index[i]==1)
			continue; // skip default words created when dic init?
		if ((find_symbol(model->forward, model->dictionary->index[i]) == NULL) && (find_symbol(model->backward, model->dictionary->index[i]) == NULL)) { // symbol
			free_word(model->dictionary->entry[model->dictionary->index[i]]);  // symbol
			model->dictionary->entry[model->dictionary->index[i]].word = NULL; // symbol
			if(tmp>0)
				syms = (int *)nrealloc((int *)(syms), sizeof(int)*(tmp+1));
			syms[tmp] = model->dictionary->index[i];
			smallest = MIN(model->dictionary->index[i], smallest);
			model->dictionary->index[i] = model->dictionary->size; // index!
			tmp++;
		}
	}

	// only if there is anything to trim:
	if(tmp) {
		/* change all references to words/symbols that were located above the deleted entries
		   this includes the model branches and the index and the phrases!
		   Do this according to the smallest entry and decreasing anything above it by the amount of entries */
		recurse_tree_and_decrement_symbols(model->forward, smallest, tmp, syms);
		recurse_tree_and_decrement_symbols(model->backward, smallest, tmp, syms);

		for(j=0; j<model->dictionary->size; ++j) //  // iterate over index
			if ((model->dictionary->index[j] > smallest) && (model->dictionary->index[j] != model->dictionary->size))
				model->dictionary->index[j] -= amount_bigger_than(syms, tmp, model->dictionary->index[j]);

		for(j=0; j<model->phrasecount; ++j)
			for(k=1; k<=model->phrase[j][0]; ++k)
				if(model->phrase[j][k] > smallest)
					model->phrase[j][k] -= amount_bigger_than(syms, tmp, model->phrase[j][k]);

		// now is a safe time to actually remove the dictionary entries as well (shift them all down)
		tmp2 = 0;
		for(i=0; i<model->dictionary->size; ++i) {
			if(model->dictionary->entry[i].word == NULL) {
				tmp2++;
				continue;
			}
			if(tmp2 == 0) continue;
			model->dictionary->entry[i-tmp2] = model->dictionary->entry[i];
		}

		// next, shift all the index entries down to replace the empty ones
		tmp2 = 0;
		for(i=0; i<model->dictionary->size; ++i) {
			if(model->dictionary->index[i] == model->dictionary->size) {
				tmp2++;
				continue;
			}
			if(tmp2 == 0) continue;
			model->dictionary->index[i-tmp2] = model->dictionary->index[i];
		}

		// resize the dictionary and reallocate the mem
		model->dictionary->size -= tmp;
		if(realloc_dictionary(model->dictionary) == NULL) {
			error("trimdictionary", "Unable to reallocate dictionary.");
			nfree(syms);
			return;
		}
	}
	nfree(syms);
}

static int amount_bigger_than(int *syms, int size, int sym)
{
	register int i, j=0;

	Context;
	for(i=0; i<size; i++)
		if(sym > syms[i])
			j++;

	return j;
}

/* This recurses through the model and decrements the symbols that are above the specified symbol
   used when an entry in the dictionary is deleted and everything is shifted down for example. */
static void recurse_tree_and_decrement_symbols(TREE *node, int smallestsymbol, int amount, int *syms)
{
	register int i;

	Context;
	if (node->symbol > smallestsymbol) {
		node->symbol -= amount_bigger_than(syms, amount, node->symbol);
}
	for(i=0; i<node->branch; ++i)
		recurse_tree_and_decrement_symbols(node->tree[i], smallestsymbol, amount, syms);
}

static int tcl_savebrain STDVAR
{
	Context;
	putlog(LOG_MISC, "*", "Saving brain...");
	save_model("megahal.brn", model);
	return TCL_OK;
}

static int tcl_reloadbrain STDVAR
{
	Context;
	setlocale(LC_ALL, "");
	char *resources = argc >= 2 ? argv[1] : NULL;
	char *cache = argc >= 3 ? argv[2] : NULL;
	change_personality(&model, resources, cache);
	putlog(LOG_MISC, "*", "Brain reloaded");
	return TCL_OK;
}

static int tcl_setmegabotnick STDVAR
{
	Context;
	setlocale(LC_ALL, "");
	BADARGS(2, 2, " <botnick>");
	wcscpy(mbotnick,locale_to_wchar(argv[1]));
	mystrlwr(mbotnick);
	return TCL_OK;
}

static int tcl_learningmode STDVAR
{
	Context;
	BADARGS(2, 2, " on/off");
	if (!strcasecmp(argv[1], "off")) {
		learningmode = FALSE;
		putlog(LOG_MISC, "*", "Brain learning mode set to off");
	} else {
		learningmode = TRUE;
		putlog(LOG_MISC, "*", "Brain learning mode set to on");
	}
	return TCL_OK;
}

static int tcl_talkfrequency STDVAR
{
	Context;
	BADARGS(2, 2, " <#oflines>");
	if (argv[1]) {
		talkfrequency = atoi(argv[1]);
		putlog(LOG_MISC, "*", "Talk frequency set to %d", atoi(argv[1]));
	}
	return 0;
}

static DICTIONARY *realloc_dictionary(DICTIONARY *dictionary)
{
	Context;
	if(dictionary->index == NULL)
		dictionary->index = (BYTE2 *)nmalloc(sizeof(BYTE2)*(dictionary->size));
	else
		dictionary->index = (BYTE2 *)nrealloc((BYTE2 *)(dictionary->index),sizeof(BYTE2)*(dictionary->size));

	if(dictionary->index == NULL)
		return NULL;

	if(dictionary->entry == NULL)
		dictionary->entry = (STRING *)nmalloc(sizeof(STRING)*(dictionary->size));
	else
		dictionary->entry = (STRING *)nrealloc((STRING *)(dictionary->entry),sizeof(STRING)*(dictionary->size));
	if(dictionary->entry == NULL)
		return NULL;

	return dictionary;
}

static TREE *realloc_tree(TREE *tree)
{
	Context;
	if(tree->branch == 0 && tree->tree != NULL) {
		nfree(tree->tree);
		tree->tree = NULL;
	} else if(tree->tree == NULL)
		tree->tree = (TREE **)nmalloc(sizeof(TREE *)*(tree->branch));
	else
		tree->tree = (TREE **)nrealloc((TREE **)(tree->tree),sizeof(TREE *)*(tree->branch));

	if(tree->tree == NULL)
		return NULL;

	return tree;
}

static BYTE2 **realloc_phrase(MODEL *model)
{
	Context;
	if(model->phrasecount == 0 && model->phrase != NULL) {
		nfree(model->phrase);
		model->phrase = NULL;
	} else if(model->phrase == NULL) {
		model->phrase = (BYTE2 **)nmalloc(sizeof(BYTE2 *)*(model->phrasecount));
	} else {
		model->phrase = (BYTE2 **)nrealloc((BYTE2 **)(model->phrase),sizeof(BYTE2 *)*(model->phrasecount));
	}

	if(model->phrase == NULL) {
		return NULL;
	}

	return model->phrase;

}

static bool isrepeating(DICTIONARY *replywords)
{
	register int i, j, k;
	bool same;

	Context;
	if (replywords->size <= model->order*2+1)
		return FALSE;

	for (i=model->order+1; i<replywords->size-model->order; i++) {
		for (j=0; j<i-model->order; j++) {
			same = TRUE;
			for (k=0; k<=model->order; k++) {
				if (wordcmp(replywords->entry[i+k], replywords->entry[j+k]) != 0) {
					same = FALSE;
					break;
				}
			}
			if (same)
				return TRUE;
		}
	}

	return FALSE;
}

static void strip_codes(wchar_t *text)
{
	wchar_t *dd = text;

	Context;
	while (*text) {
		switch (*text) {
		case 2:						/* Bold text */
			text++;
			continue;
		case 3:						/* mIRC colors? */
			if (iswdigit(text[1])) {		/* Is the first wchar_t a number? */
				text += 2;			/* Skip over the ^C and the first digit */
				if (iswdigit(*text))
					text++;			/* Is this a double digit number? */
				if (*text == L',') {		/* Do we have a background color next? */
					if (iswdigit(text[1]))
						text += 2;	/* Skip over the first background digit */
					if (iswdigit(*text))
						text++;		/* Is it a double digit? */
				}
			} else
				text++;
			continue;
		case 7:
			text++;
			continue;
		case 0x16:					/* Reverse video */
			text++;
			continue;
		case 0x1f:					/* Underlined text */
			text++;
			continue;
		case 033:
			text++;
			if (*text == L'[') {
				text++;
				while ((*text == L';') || iswdigit(*text))
					text++;
				if (*text)
					text++;			/* also kill the following char */
			}
			continue;
		}
		*dd++ = *text++;				/* Move on to the next char */
	}
	*dd = 0;
}

static bool isinprevs(DICTIONARY *words)
{
	Context;
	if (dissimilar2(words, prev1) && dissimilar2(words, prev2) && dissimilar2(words, prev3) &&
	    dissimilar2(words, prev4) && dissimilar2(words, prev5))
		return FALSE;
	return TRUE;
}

static bool dissimilar2(DICTIONARY *words1, DICTIONARY *words2)
{
	register int i, j;
	int count = 0;

	Context;
	for(i=0; i<words1->size; ++i) {
		for(j=0; j<words2->size; ++j) {
			if(wordcmp(words1->entry[i], words2->entry[j])==0) {
				count++;
				break;
			}
		}
	}
	if (count>(words1->size/4*3))
		return FALSE;
	return TRUE;
}

static void updateprevs(wchar_t *words)
{
	Context;
	free_words(prev1);
	free_dictionary(prev1);
	nfree(prev1);

	prev1 = prev2;
	prev2 = prev3;
	prev3 = prev4;
	prev4 = prev5;

	prev5 = new_dictionary();
	make_words(words, prev5);
}

/*
 * The remainder of this code was just pasted from the rest of megahal.c, and
 * modified where necessary to get messages put in the right place, etc.
 */

/*---------------------------------------------------------------------------*/
/*
 *	Function:	Error
 *
 *	Purpose:	Print the specified message to the error file.
 */
static void error(char *title, char *fmt, ...)
{
	va_list argp;
	char stuff[512];

	Context;
	sprintf(stuff, "%s: ", title);
	va_start(argp, fmt);
	vsprintf(stuff, fmt, argp);
	va_end(argp);
	sprintf(stuff, ".\n");
	putlog(LOG_MISC, "*", "%s", stuff);

	/* FIXME - I think I need to die here */
}

/*---------------------------------------------------------------------------*/

static bool warn(char *title, char *fmt, ...)
{
	va_list argp;
	char stuff[512];

	Context;
	sprintf(stuff, "%s: ", title);
	va_start(argp, fmt);
	vsprintf(stuff, fmt, argp);
	va_end(argp);
	sprintf(stuff, ".\n");
	putlog(LOG_MISC, "*", "%s", stuff);
	return TRUE;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Capitalize
 *
 *	Purpose:	Convert a string to look nice.
 */
static void capitalize(wchar_t *string)
{
	register size_t i;
	bool start = TRUE;

	Context;
	for(i=0; i<wcslen(string); ++i) {
		if(iswalpha(string[i])) {
			if(start == TRUE)
				string[i] = (wchar_t)towupper(string[i]);
			else
				string[i] = (wchar_t)towlower(string[i]);
			start = FALSE;
		}
		if((i>2)&&(wcschr(L"!.?", string[i-1])!=NULL)&&(iswspace(string[i])))
			start = TRUE;
	}
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Upper
 *
 *	Purpose:	Convert a string to its uppercase representation.
 */
static void upper(wchar_t *string)
{
	register size_t i;

	Context;
	for(i=0; i<wcslen(string); ++i)
		string[i] = (wchar_t)towupper(string[i]);
}

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Add_Word
 *
 *	Purpose:	Add a word to a dictionary, and return the identifier
 *			assigned to the word.  If the word already exists in
 *			the dictionary, then return its current identifier
 *			without adding it again.
 */
static BYTE2 add_word(DICTIONARY *dictionary, STRING word)
{
	register int i;
	int position;
	bool found;

	Context;
	/*
	 *	If the word's already in the dictionary, there is no need to add it
	 */
	position = search_dictionary(dictionary, word, &found);
	if(found == TRUE)
		goto succeed;

	/*
	 *	Increase the number of words in the dictionary
	 */
	dictionary->size += 1;

	/*
	 *	Allocate one more entry for the word index
	 */
	if(realloc_dictionary(dictionary) == NULL) {
		error("add_word", "Unable to reallocate the dictionary.");
		goto fail;
	}

	/*
	 *	Copy the new word into the word array
	 */
	dictionary->entry[dictionary->size-1].length = word.length;
	dictionary->entry[dictionary->size-1].word = (wchar_t *)nmalloc(sizeof(wchar_t)*(word.length));
	if(dictionary->entry[dictionary->size-1].word == NULL) {
		error("add_word", "Unable to allocate the word.");
		goto fail;
	}
	for(i=0; i<word.length; ++i)
		dictionary->entry[dictionary->size-1].word[i] = word.word[i];

	/*
	 *	Shuffle the word index to keep it sorted alphabetically
	 */
	for(i=(dictionary->size-1); i>position; --i)
		dictionary->index[i] = dictionary->index[i-1];

	/*
	 *	Copy the new symbol identifier into the word index
	 */
	dictionary->index[position] = dictionary->size-1;

succeed:
	return dictionary->index[position];

fail:
	return 0;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Search_Dictionary
 *
 *	Purpose:	Search the dictionary for the specified word, returning its
 *			position in the index if found, or the position where it
 *			should be inserted otherwise.
 */
static int search_dictionary(DICTIONARY *dictionary, STRING word, bool *find)
{
	int position;
	int min;
	int max;
	int middle;
	int compar;

	Context;
	/*
	 *	If the dictionary is empty, then obviously the word won't be found
	 */
	if(dictionary->size == 0) {
		position = 0;
		goto notfound;
	}

	/*
	 *	Initialize the lower and upper bounds of the search
	 */
	min = 0;
	max = dictionary->size-1;
	/*
	 *	Search repeatedly, halving the search space each time, until either
	 *	the entry is found, or the search space becomes empty
	 */
	while(TRUE) {
		/*
		 *	See whether the middle element of the search space is greater
		 *	than, equal to, or less than the element being searched for.
		 */
		middle = (min+max)/2;
		compar = wordcmp(word, dictionary->entry[dictionary->index[middle]]);
		/*
		 *	If it is equal then we have found the element.  Otherwise we
		 *	can halve the search space accordingly.
		 */
		if(compar == 0) {
			position = middle;
			goto found;
		} else if(compar > 0) {
			if(max == middle) {
				position = middle+1;
				goto notfound;
			}
			min = middle+1;
		} else {
			if(min == middle) {
				position = middle;
				goto notfound;
			}
			max = middle-1;
		}
	}

found:
	*find=TRUE;
	return position;

notfound:
	*find=FALSE;
	return position;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Find_Word
 *
 *	Purpose:	Return the symbol corresponding to the word specified.
 *			We assume that the word with index zero is equal to a
 *			NULL word, indicating an error condition.
 */
static BYTE2 find_word(DICTIONARY *dictionary, STRING word)
{
	int position;
	bool found;

	Context;
	position = search_dictionary(dictionary, word, &found);

	if(found == TRUE)
		return dictionary->index[position];
	else
		return 0;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Wordcmp
 *
 *	Purpose:	Compare two words, and return an integer indicating whether
 *			the first word is less than, equal to or greater than the
 *			second word.
 */
static int wordcmp(STRING word1, STRING word2)
{
	register int i;
	int bound;

	bound = MIN(word1.length,word2.length);

	for(i=0; i<bound; ++i)
		if(towupper(word1.word[i])!=towupper(word2.word[i]))
			return (int)(towupper(word1.word[i])-towupper(word2.word[i]));

	if(word1.length<word2.length)
		return -1;
	if(word1.length>word2.length)
		return 1;

	return 0;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Wordcmp
 *
 *	Purpose:	Compare two words, and return an integer indicating whether
 *			the first word is less than, equal to or greater than the
 *			second word.
 */
static int wordcmp2(STRING word1, wchar_t *word2)
{
	register int i;
	int bound, length2;

	length2 = wcslen(word2);
	bound = MIN(word1.length,length2);

	for(i=0; i<bound; ++i)
		if(towupper(word1.word[i])!=towupper(word2[i]))
			return (int)(towupper(word1.word[i])-towupper(word2[i]));

	if(word1.length<length2)
		return -1;
	if(word1.length>length2)
		return 1;

	return 0;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Free_Dictionary
 *
 *	Purpose:	Release the memory consumed by the dictionary.
 */
static void free_dictionary(DICTIONARY *dictionary)
{
	Context;
	if(dictionary == NULL)
		return;
	if(dictionary->entry != NULL) {
		nfree(dictionary->entry);
		dictionary->entry = NULL;
	}
	if(dictionary->index != NULL) {
		nfree(dictionary->index);
		dictionary->index = NULL;
	}
	dictionary->size = 0;
}

/*---------------------------------------------------------------------------*/

static void free_model(MODEL *model)
{
	register int i;

	Context;
	if(model == NULL)
		return;
	if(model->forward != NULL) {
		free_tree(model->forward);
	}
	if(model->backward != NULL) {
		free_tree(model->backward);
	}
	if(model->halcontext != NULL) {
		nfree(model->halcontext);
	}
	for (i=0; i<model->phrasecount; i++)
		nfree(model->phrase[i]);
	if(model->phrase != NULL)
		nfree(model->phrase);
	if(model->dictionary != NULL) {
		free_words(model->dictionary);
		free_dictionary(model->dictionary);
		nfree(model->dictionary);
	}
	nfree(model);
}

/*---------------------------------------------------------------------------*/

static void free_tree(TREE *tree)
{
	static int level = 0;
	register int i;

	Context;
	if(tree == NULL)
		return;

	if(tree->tree!=NULL) {
		for(i=0; i<tree->branch; ++i) {
			++level;
			free_tree(tree->tree[i]);
			--level;
		}
		nfree(tree->tree);
	}
	nfree(tree);
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Initialize_Dictionary
 *
 *	Purpose:	Add dummy words to the dictionary.
 */
static void initialize_dictionary(DICTIONARY *dictionary)
{
	STRING word = { 12, L"<BRAINSTART>" };
	STRING end = { 5, L"<FIN>" };

	Context;
	(void)add_word(dictionary, word);
	(void)add_word(dictionary, end);
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	New_Dictionary
 *
 *	Purpose:	Allocate room for a new dictionary.
 */
static DICTIONARY *new_dictionary(void)
{
	DICTIONARY *dictionary = NULL;

	Context;
	dictionary = (DICTIONARY *)nmalloc(sizeof(DICTIONARY));
	if(dictionary == NULL) {
		error("new_dictionary", "Unable to allocate dictionary.");
		return NULL;
	}

	dictionary->size = 0;
	dictionary->index = NULL;
	dictionary->entry = NULL;

	return dictionary;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Save_Dictionary
 *
 *	Purpose:	Save a dictionary to the specified file.
 */
static void save_dictionary(FILE *file, DICTIONARY *dictionary)
{
	register int i;

	Context;
	fwrite(&(dictionary->size), sizeof(BYTE4), 1, file);
	for(i=0; i<dictionary->size; ++i)
		save_word(file, dictionary->entry[i]);
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Load_Dictionary
 *
 *	Purpose:	Load a dictionary from the specified file.
 */
static void load_dictionary(FILE *file, DICTIONARY *dictionary)
{
	register int i;
	int size;

	Context;
	if ( fread(&size, sizeof(BYTE4), 1, file) ) {
		for(i=0; i<size; ++i) {
			load_word(file, dictionary);
		}
	}
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Save_Word
 *
 *	Purpose:	Save a dictionary word to a file.
 */
static void save_word(FILE *file, STRING word)
{
	register int i;

	Context;
	fwrite(&(word.length), sizeof(BYTE1), 1, file);
	for(i=0; i<word.length; ++i)
		fwrite(&(word.word[i]), sizeof(wchar_t), 1, file);
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Load_Word
 *
 *	Purpose:	Load a dictionary word from a file.
 */
static void load_word(FILE *file, DICTIONARY *dictionary)
{
	register int i;
	STRING word;

	Context;
	if ( fread(&(word.length), sizeof(BYTE1), 1, file) ) {
		word.word = (wchar_t *)nmalloc(sizeof(wchar_t)*word.length);
		if(word.word == NULL) {
			error("load_word", "Unable to allocate word");
			return;
		}
		for(i=0; i<word.length; ++i) {
			if (!fread(&(word.word[i]), sizeof(wchar_t), 1, file) ) {
				break;
			}
		}
		add_word(dictionary, word);
		nfree(word.word);
	}
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	New_Node
 *
 *	Purpose:	Allocate a new node for the n-gram tree, and initialise
 *			its contents to sensible values.
 */
static TREE *new_node(void)
{
	TREE *node = NULL;

	Context;
	/*
	 *	Allocate memory for the new node
	 */
	node = (TREE *)nmalloc(sizeof(TREE));
	if(node == NULL) {
		error("new_node", "Unable to allocate the node.");
		goto fail;
	}

	/*
	 *	Initialise the contents of the node
	 */
	node->symbol = 0;
	node->usage = 0;
	node->count = 0;
	node->branch = 0;
	node->tree = NULL;

	return node;

fail:
	if(node != NULL)
		nfree(node);
	return NULL;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	New_Model
 *
 *	Purpose:	Create and initialise a new ngram model.
 */
static MODEL *new_model(int order)
{
	MODEL *model = NULL;

	Context;
	model = (MODEL *)nmalloc(sizeof(MODEL));
	if(model == NULL) {
		error("new_model", "Unable to allocate model.");
		goto fail;
	}

	model->order = order;
	model->forward = new_node();
	model->backward = new_node();
	model->halcontext = (TREE **)nmalloc(sizeof(TREE *)*(order+2));
	if(model->halcontext == NULL) {
		error("new_model", "Unable to allocate context array.");
		goto fail;
	}
	initialize_context(model);
	model->phrasecount = 0;
	model->phrase = NULL;
	model->dictionary = new_dictionary();
	initialize_dictionary(model->dictionary);

	return model;

fail:
	return NULL;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Update_Model
 *
 *	Purpose:	Update the model with the specified symbol.
 */
static void update_model(MODEL *model, int symbol)
{
	register int i;

	Context;
	/*
	 *	Update all of the models in the current context with the specified
	 *	symbol.
	 */

	// add the symbol to all contexts - some may be at different levels deep and there may be max [order] different branches being built at once
	// Question: Why does it go up to order+1? Nothing ever uses that halcontext as far as I can see.Maybe its only here so that the i-1 can be 'add_symbol'ed within the same loop and the programmer was lazy ;)
	for(i=(model->order+1); i>0; --i)
		if(model->halcontext[i-1] != NULL)
			model->halcontext[i] = add_symbol(model->halcontext[i-1], (BYTE2)symbol);

	return;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Update_Context
 *
 *	Purpose:	Update the context of the model without adding the symbol.
 */
static void update_context(MODEL *model, int symbol)
{
	register int i;

	Context;
	for(i=(model->order+1); i>0; --i)
		if(model->halcontext[i-1] != NULL)
			model->halcontext[i] = find_symbol(model->halcontext[i-1], symbol);
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Add_Symbol
 *
 *	Purpose:	Update the statistics of the specified tree with the
 *			specified symbol, which may mean growing the tree if the
 *			symbol hasn't been seen in this context before.
 */
static TREE *add_symbol(TREE *tree, BYTE2 symbol)
{
	TREE *node=NULL;

	Context;
	/*
	 *	Search for the symbol in the subtree of the tree node.
	 */
	node = find_symbol_add(tree, symbol);

	/*
	 *	Increment the symbol counts
	 */
	if((node->count < 65535)) {
		node->count += 1;
		tree->usage += 1;
	}

	return node;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Find_Symbol
 *
 *	Purpose:	Return a pointer to the child node, if one exists, which
 *			contains the specified symbol.
 */
static TREE *find_symbol(TREE *node, int symbol)
{
	register int i;
	TREE *found = NULL;
	bool found_symbol = FALSE;

	Context;
	/*
	 *	Perform a binary search for the symbol.
	 */
	i = search_node(node, symbol, &found_symbol);
	if(found_symbol == TRUE)
		found=node->tree[i];

	return found;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Find_Symbol_Add
 *
 *	Purpose:	This function is conceptually similar to find_symbol,
 *			apart from the fact that if the symbol is not found,
 *			a new node is automatically allocated and added to the
 *			tree.
 */
static TREE *find_symbol_add(TREE *node, int symbol)
{
	register int i;
	TREE *found = NULL;
	bool found_symbol = FALSE;

	Context;
	/*
	 *		Perform a binary search for the symbol.  If the symbol isn't found,
	 *		attach a new sub-node to the tree node so that it remains sorted.
	 */
	i = search_node(node, symbol, &found_symbol);
	if(found_symbol == TRUE) {
		found = node->tree[i];
	} else {
		found = new_node();
		found->symbol = symbol;
		add_node(node, found, i);
	}

	return found;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Add_Node
 *
 *	Purpose:	Attach a new child node to the sub-tree of the tree
 *			specified.
 */
static void add_node(TREE *tree, TREE *node, int position)
{
	register int i;

	Context;
	/*
	 *	Allocate room for one more child node, which may mean allocating
	 *	the sub-tree from scratch.
	 */
	tree->branch += 1;
	if(realloc_tree(tree) == NULL) {
		error("add_node", "Unable to reallocate subtree.");
		return;
	}

	/*
	 *	Shuffle the nodes down so that we can insert the new node at the
	 *	subtree index given by position.
	 */
	for(i=tree->branch-1; i>position; --i)
		tree->tree[i] = tree->tree[i-1];

	/*
	 *	Add the new node to the sub-tree.
	 */
	tree->tree[position]=node;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Search_Node
 *
 *	Purpose:	Perform a binary search for the specified symbol on the
 *			subtree of the given node.  Return the position of the
 *			child node in the subtree if the symbol was found, or the
 *			position where it should be inserted to keep the subtree
 *			sorted if it wasn't.
 */
static int search_node(TREE *node, int symbol, bool *found_symbol)
{
	register int position;
	int min;
	int max;
	int middle;
	int compar;

	Context;
	/*
	 *	Handle the special case where the subtree is empty.
	 */
	if(node->branch == 0) {
		position = 0;
		goto notfound;
	}

	/*
	 *	Perform a binary search on the subtree.
	 */
	min = 0;
	max = node->branch-1;
	while(TRUE) {
		middle = (min+max)/2;
		compar = symbol-node->tree[middle]->symbol;
		if(compar == 0) {
			position = middle;
			goto found;
		} else if(compar > 0) {
			if(max == middle) {
				position = middle+1;
				goto notfound;
			}
			min = middle+1;
		} else {
			if(min == middle) {
				position = middle;
				goto notfound;
			}
			max = middle-1;
		}
	}

found:
	*found_symbol = TRUE;
	return position;

notfound:
Context;
	*found_symbol = FALSE;
	return position;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Initialize_Context
 *
 *	Purpose:	Set the context of the model to a default value.
 */
static void initialize_context(MODEL *model)
{
	register int i;

	Context;
	for(i=0; i<=model->order; ++i)
		model->halcontext[i] = NULL;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Learn
 *
 *	Purpose:	Learn from the user's input.
 */
static void learn(MODEL *model, DICTIONARY *words)
{
	register int i;
	BYTE2 symbol;
	bool nospace = TRUE;

	Context;
	/*
	 *	We only learn from inputs which are long enough
	 */
	if(words->size <= (model->order))
		return;

	// check if there are spaces in the word or its merely one word+punctuation
	for(i=1; i<words->size; i++)
		if(words->entry[i].word[0] != (wchar_t)31) {
			nospace = FALSE;
			break;
		}
	if (nospace)
		return;

	// Add a new phrase to the model
	model->phrasecount++;
	if(realloc_phrase(model)==NULL) {
		error("learn", "Unable to reallocate phrase");
		return;
	}
	model->phrase[model->phrasecount-1]=(BYTE2 *)nmalloc(sizeof(BYTE2)*(words->size+2));
	if (model->phrase[model->phrasecount-1] == NULL) {
		error("learn", "Unable to allocate phrase");
		return;
	}
	model->phrase[model->phrasecount-1][0] = words->size+1;

	/*
	 *	Train the model in the forwards direction.  Start by initializing
	 *	the context of the model.
	 */
	initialize_context(model);
	model->halcontext[0] = model->forward;
	for(i=0; i<words->size; ++i) {
		/*
		 *	Add the symbol to the model's dictionary if necessary, and then
		 *	update the forward model accordingly.
		 */
		symbol = add_word(model->dictionary, words->entry[i]);
		update_model(model, symbol);
		model->phrase[model->phrasecount-1][i+1] = symbol;
	}
	/*
	 *	Add the sentence-terminating symbol.
	 */
	update_model(model, 1);
	model->phrase[model->phrasecount-1][words->size+1] = 1;

	/*
	 *	Train the model in the backwards direction.  Start by initializing
	 *	the context of the model.
	 */
	initialize_context(model);
	model->halcontext[0] = model->backward;
	for(i=words->size-1; i>=0; --i) {
		/*
		 *	Find the symbol in the model's dictionary, and then update
		 *	the backward model accordingly.
		 */
		symbol = find_word(model->dictionary, words->entry[i]);
		update_model(model, symbol);
	}
	/*
	 *	Add the sentence-terminating symbol.
	 */
	update_model(model, 1);

	return;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Train
 *
 *	Purpose:	Infer a MegaHAL brain from the contents of a text file.
 */
static void train(MODEL *model, char *filename)
{
	FILE *file;
	char buffer[1024];
	wchar_t *wbuffer = NULL;
	DICTIONARY *words = NULL;
	int length;

	Context;
	if(filename == NULL)
		return;

	file=fopen(filename, "r");
	if(file == NULL) {
		putlog(LOG_MISC, "*", "Unable to find the personality %s\n", filename);
		return;
	}

	fseek(file, 0, 2);
	length = ftell(file);
	rewind(file);

	words = new_dictionary();

	while(!feof(file)) {

		if(fgets(buffer, 1024, file)==NULL)
			break;
		if(buffer[0] == '#')
			continue; // comments

		wbuffer=locale_to_wchar(buffer);
		wbuffer[wcslen(wbuffer)-1] = L'\0';

		upper(wbuffer);
		make_words(wbuffer, words);
		learn(model, words);
		nfree(wbuffer);

	}

	free_dictionary(words);
	fclose(file);
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Show_Dictionary
 *
 *	Purpose:	Display the dictionary for training purposes.
 */
static void show_dictionary(DICTIONARY *dictionary)
{
	register int i;
	register int k;
	FILE *file;
	char *ldict_word;
	wchar_t *tmp;
	char filename[512];

	Context;
	snprintf(filename, sizeof(filename), "%s%smegahal.dic", directory_cache, SEP);
	file = fopen(filename, "w");
	if(file == NULL) {
		warn("show_dictionary", "Unable to open file");
		return;
	}

	for(i=0; i<dictionary->size; ++i) {
		tmp = (wchar_t *)nmalloc(sizeof(wchar_t)*(dictionary->entry[i].length+1));
		for(k=0;k<dictionary->entry[i].length;k++)
			tmp[k] = dictionary->entry[i].word[k];
		tmp[dictionary->entry[i].length] = L'\0';
		ldict_word = wchar_to_locale(tmp);
		fputs(ldict_word, file);
		nfree(tmp);
		nfree(ldict_word);
		fprintf(file, "\n");
	}

	fclose(file);
}

static void save_phrases(MODEL *model)
{
	register int i, j;
	DICTIONARY *phrase;
	FILE *file;
	char *phrase2;
	char filename[512];

	Context;
	phrase = new_dictionary();
	snprintf(filename, sizeof(filename), "%s%smegahal.phr", directory_cache, SEP);
	file = fopen(filename, "w");
	if(file == NULL) {
		warn("save_phrases", "Unable to open file");
		return;
	}

	for(i=0; i<model->phrasecount; ++i) {

		phrase->size = model->phrase[i][0]-1;
		if(realloc_dictionary(phrase) == NULL) {
			error("save_phrases", "Unable to reallocate dictionary");
			return;
		}
		for(j=0; j<phrase->size; ++j)
			phrase->entry[j] = model->dictionary->entry[model->phrase[i][j+1]];

		phrase2 = wchar_to_locale(make_output(phrase));
		fputs(phrase2, file);
		fprintf(file, "\n");
		nfree(phrase2);
	}

	fclose(file);
	free_dictionary(phrase);
}


/*---------------------------------------------------------------------------*/

/*
 *	Function:	Save_Model
 *
 *	Purpose:	Save the current state to a MegaHAL brain file.
 */
static void save_model(char *modelname, MODEL *model)
{
	register int i, j;
	FILE *file;
  char filename[512];

	Context;

	show_dictionary(model->dictionary);
	save_phrases(model);

	snprintf(filename, sizeof(filename), "%s%s%s", directory_cache, SEP, modelname);
	file = fopen(filename, "wb");
	if(file == NULL) {
		warn("save_model", "Unable to open file `%s'", filename);
		return;
	}

	fwrite(_T(COOKIE), sizeof(wchar_t), wcslen(_T(COOKIE)), file);
	fwrite(&(model->order), sizeof(BYTE1), 1, file);
	save_tree(file, model->forward);
	save_tree(file, model->backward);
	save_dictionary(file, model->dictionary);
	fwrite(&(model->phrasecount), sizeof(BYTE4), 1, file);
	for(i=0; i<model->phrasecount; ++i)
		for(j=0; j<model->phrase[i][0]+1; ++j)
			fwrite(&(model->phrase[i][j]), sizeof(BYTE2), 1, file);
	fclose(file);
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Save_Tree
 *
 *	Purpose:	Save a tree structure to the specified file.
 */
static void save_tree(FILE *file, TREE *node)
{
	static int level=0;
	register int i;

	Context;
	fwrite(&(node->symbol), sizeof(BYTE2), 1, file);
	fwrite(&(node->usage), sizeof(BYTE4), 1, file);
	fwrite(&(node->count), sizeof(BYTE2), 1, file);
	fwrite(&(node->branch), sizeof(BYTE2), 1, file);

	for(i=0; i<node->branch; ++i) {
		++level;
		save_tree(file, node->tree[i]);
		--level;
	}
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Load_Tree
 *
 *	Purpose:	Load a tree structure from the specified file.
 */
static void load_tree(FILE *file, TREE *node)
{
	static int level = 0;
	register int i;

	Context;
	if ( fread(&(node->symbol), sizeof(BYTE2), 1, file) &&
	     fread(&(node->usage), sizeof(BYTE4), 1, file) &&
	     fread(&(node->count), sizeof(BYTE2), 1, file) &&
	     fread(&(node->branch), sizeof(BYTE2), 1, file) ) {

		if(node->branch==0) {
			return;
		}

		node->tree = (TREE **)nmalloc(sizeof(TREE *)*(node->branch));
		if(node->tree == NULL) {
			error("load_tree", "Unable to allocate subtree");
			return;
		}

		for(i=0; i<node->branch; ++i) {
			node->tree[i] = new_node();
			++level;
			load_tree(file, node->tree[i]);
			--level;
		}
	}
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Load_Model
 *
 *	Purpose:	Load a model into memory.
 */
static bool load_model(char *filename, MODEL *model)
{
	register int i, j;
	BYTE2 size;
	FILE *file;
	wchar_t cookie[16];

	Context;
	if(filename == NULL)
		return FALSE;

	file = fopen(filename, "rb");
	if(file == NULL) {
		warn("load_model", "Unable to open file `%s'", filename);
		return FALSE;
	}

	if (!fread(cookie, sizeof(wchar_t), wcslen(_T(COOKIE)), file) ||
	    wcsncmp(cookie, _T(COOKIE), wcslen(_T(COOKIE))) != 0 ||
	    !fread(&(model->order), sizeof(BYTE1), 1, file)
	   ) {
		warn("load_model", "File `%s' is not a MegaHAL brain", filename);
		goto fail;
	}

	order = model->order;
	load_tree(file, model->forward);
	load_tree(file, model->backward);
	load_dictionary(file, model->dictionary);

	if ( !fread(&(model->phrasecount), sizeof(BYTE4), 1, file) ||
	realloc_phrase(model) == NULL ) {
		error("load_model", "Unable to reallocate phrase");
		return FALSE;
	}
	for(i=0; i<model->phrasecount; ++i) {
		if ( fread(&size, sizeof(BYTE2), 1, file) ) {
			model->phrase[i]=(BYTE2 *)nmalloc(sizeof(BYTE2)*(size+2));
			if (model->phrase[i] == NULL) {
				error("learn", "Unable to allocate phrase");
				return FALSE;
			}
		}
		model->phrase[i][0] = size;
		for(j=0; j<size; ++j) {
			if (!fread(&(model->phrase[i][j+1]), sizeof(BYTE2), 1, file)) {
				break;
			}
		}
		model->phrase[i][size+1] = 1; // terminator
	}

	return TRUE;
fail:
	fclose(file);

	return FALSE;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Make_Words
 *
 *	Purpose:	Break a string into an array of words.
 */
static void make_words(wchar_t *pinput, DICTIONARY *words)
{
	size_t offset = 0, tmp = 0;
	register int i;
	wchar_t iinput[512] = L"";
	wchar_t *input = iinput;

	Context;
	/*
	 *	Clear the entries in the dictionary
	 */
	free_words(words);
	free_dictionary(words);
	wcscpy(iinput,pinput);
	strip_codes(input);
	/*
	 *	If the string is empty then do nothing, for it contains no words.
	 */
	if(wcslen(input) == 0)
		return;
	/*
	 *	Loop forever.
	 */
	while(1) {

		/*
		 *	If the current character is of the same type as the previous
		 *	character, then include it in the word.  Otherwise, terminate
		 *	the current word.
		 */
		while(input[0] == L' ')
			input++;
		if(!input[0])
			break;
		if((boundary(input, offset) || (input[offset] == L' '))) {

			/*
			 *	Add the word to the dictionary
			 */
			words->size += 1;
			if(realloc_dictionary(words) == NULL) {
				error("make_words", "Unable to reallocate dictionary");
				return;
			}
			if(((input-1)[0] != L' ') && (words->size>1))
				tmp = 1;
			words->entry[words->size-1].length = offset+tmp;
			words->entry[words->size-1].word = (wchar_t *)nmalloc(sizeof(wchar_t)*(offset+tmp));
			if(tmp)
				words->entry[words->size-1].word[0]=(wchar_t)31;
			for(i=0; i<offset; i++)
				words->entry[words->size-1].word[i+tmp]=input[i];
			if(offset == wcslen(input))
				break;
			input += offset;
			offset = tmp = 0;
		} else {
			++offset;
		}
	}
	/*
	 *	If the last word isn't punctuation, then replace it with a
	 *	full-stop character.
	 */
	if (words->size == 0) {
		return;
	}

	if(iswalnum(words->entry[words->size-1].word[0]) || (words->entry[words->size-1].word[0]==(wchar_t)31 && iswalnum(words->entry[words->size-1].word[1])) ) {
		words->size += 1;
		if(realloc_dictionary(words) == NULL) {
			error("make_words", "Unable to reallocate dictionary");
			return;
		}

		words->entry[words->size-1].word = (wchar_t *)nmalloc(sizeof(wchar_t)*(2));
		words->entry[words->size-1].length = 2;
		words->entry[words->size-1].word[0] = (wchar_t)31;
		words->entry[words->size-1].word[1] = L'.';
	} else if(wcschr(L"!.?", words->entry[words->size-1].word[words->entry[words->size-1].length-1]) == NULL) {
		words->entry[words->size-1].word = (wchar_t *)nrealloc(words->entry[words->size-1].word, sizeof(wchar_t)*(2));
		words->entry[words->size-1].length = 2;
		words->entry[words->size-1].word[0] = (wchar_t)31;
		words->entry[words->size-1].word[1] = L'.';
	}
	return;
}

/*---------------------------------------------------------------------------*/
/*
 *	Function:	Boundary
 *
 *	Purpose:	Return whether or not a word boundary exists in a string
 *			at the specified location.
 */
static bool boundary(wchar_t *string, int position)
{

	Context;
	if(position == 0)
		return FALSE;

	if(position == (int)wcslen(string))
		return TRUE;

	if(
		((string[position] == L'\'')) &&
		(iswalnum(string[position-1]) != 0) &&
		(iswalnum(string[position+1]) != 0)
	)
		return FALSE;

	if(
		(position > 1) &&
		((string[position-1] == L'\'')) &&
		(iswalnum(string[position-2]) !=0) &&
		(iswalnum(string[position]) != 0)
	)
		return FALSE;

	if(
		((string[position] == L'-')) &&
		(iswalnum(string[position-1]) != 0) &&
		(iswalnum(string[position+1]) != 0)
	)
		return FALSE;

	if(
		(position > 1) &&
		((string[position-1] == L'-')) &&
		(iswalnum(string[position-2]) != 0) &&
		(iswalnum(string[position]) != 0)
	)
		return FALSE;

	if(
		(iswalnum(string[position]) != 0) &&
		(iswalnum(string[position-1]) == 0)
	)
		return TRUE;

	if(
		(iswalnum(string[position]) == 0) &&
		(iswalnum(string[position-1]) != 0)
	)
		return TRUE;

/*	if(isdigit(string[position])!=isdigit(string[position-1]))
		return(TRUE);
*/
	return FALSE;
}

/*---------------------------------------------------------------------------*/
/*
 *	Function:	Generate_Reply
 *
 *	Purpose:	Take a string of user input and return a string of output
 *			which may vaguely be construed as containing a reply to
 *			whatever is in the input string.
 */
static wchar_t *generate_reply(MODEL *model, DICTIONARY *words)
{
	static DICTIONARY *dummy = NULL;
	DICTIONARY *replywords;
	DICTIONARY *keywords;
	float surprise;
	float max_surprise;
	wchar_t *output;
	static wchar_t *output_none = NULL;
	int basetime;

	Context;
	/*
	 *	Create an array of keywords from the words in the user's input
	 */
	keywords = make_keywords(model, words);

	/*
	 *	Make sure some sort of reply exists
	 */
	if(output_none == NULL) {
		output_none = nmalloc(512);
		if(output_none != NULL)
			wcscpy(output_none, L"I don't know enough to answer you yet!");
	}
	output = output_none;
	if(dummy == NULL)
		dummy = new_dictionary();
	replywords = reply(model, dummy);
	basetime = time(NULL);
	while(((maxreplywords && replywords->size>maxreplywords) || dissimilar(words, replywords)==FALSE || isrepeating(replywords) || isinprevs(replywords)) && (time(NULL)-basetime)<timeout )
		replywords = reply(model, dummy);
	output = make_output(replywords);
	/*
	 *	Loop for the specified waiting period, generating and evaluating
	 *	replies
	 */
	max_surprise = (float)-1.0;
	basetime = time(NULL);
	do {
		replywords = reply(model, keywords);
		if ((maxreplywords && replywords->size>maxreplywords) || dissimilar(words, replywords)==FALSE ||
		    isrepeating(replywords) || isinprevs(replywords))
			continue;
		surprise = evaluate_reply(model, keywords, replywords);
		if(surprise > max_surprise) {
			max_surprise = surprise;
			output = make_output(replywords);
		}
	} while((time(NULL)-basetime) < timeout);
	updateprevs(output);

	/*
	 *	Return the best answer we generated
	 */
	return output;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Dissimilar
 *
 *	Purpose:	Return TRUE or FALSE depending on whether the dictionaries
 *			are the same or not.
 */
static bool dissimilar(DICTIONARY *words1, DICTIONARY *words2)
{
	register int i;

	Context;
	if(words1->size != words2->size)
		return TRUE;
	for(i=0; i<words1->size; ++i)
		if(wordcmp(words1->entry[i], words2->entry[i])!=0)
			return TRUE;
	return FALSE;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Make_Keywords
 *
 *	Purpose:	Put all the interesting words from the user's input into
 *			a keywords dictionary, which will be used when generating
 *			a reply.
 */
static DICTIONARY *make_keywords(MODEL *model, DICTIONARY *words)
{
	static DICTIONARY *keys=NULL;
	register int i;
	register int j;
	int c;

	Context;
	if(keys == NULL)
		keys = new_dictionary();
	free_words(keys);
	free_dictionary(keys);

	for(i=0; i<words->size; ++i) {
		/*
		 *		Find the symbol ID of the word.  If it doesn't exist in
		 *		the model, or if it begins with a non-alphanumeric
		 *		wchar_tacter, or if it is in the exclusion array, then
		 *		skip over it.
		 */
		c = 0;
		for(j=0; j<swp->size; ++j)
			if(wordcmp(swp->from[j], words->entry[i]) == 0) {
				add_key(model, keys, swp->to[j]);
				++c;
			}
		if(c == 0)
			add_key(model, keys, words->entry[i]);
	}

	if(keys->size>0)
		for(i=0; i<words->size; ++i) {
			c=0;
			for(j=0; j<swp->size; ++j)
				if(wordcmp(swp->from[j], words->entry[i]) == 0) {
					add_aux(model, keys, swp->to[j]);
					++c;
				}
			if(c == 0)
				add_aux(model, keys, words->entry[i]);
		}

	return keys;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Add_Key
 *
 *	Purpose:	Add a word to the keyword dictionary.
 */
static void add_key(MODEL *model, DICTIONARY *keys, STRING word)
{
	int symbol;
	bool fnd;

	Context;
	symbol = find_word(model->dictionary, word);
	if(symbol == 0)
		return;
	if((word.word[0]!=(wchar_t)31 && iswalnum(word.word[0])==0) || (word.word[0]==(wchar_t)31 && iswalnum(word.word[1])==0))
		return;
	search_dictionary(ban, word, &fnd);
	if(fnd)
		return;
	search_dictionary(aux, word, &fnd);
	if(fnd)
		return;

	add_word(keys, word);
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Add_Aux
 *
 *	Purpose:	Add an auxilliary keyword to the keyword dictionary.
 */
static void add_aux(MODEL *model, DICTIONARY *keys, STRING word)
{
	int symbol;
	bool fnd;

	Context;
	symbol = find_word(model->dictionary, word);
	if(symbol == 0)
		return;
	if(iswalnum(word.word[0]) == 0)
		return;
	search_dictionary(aux, word, &fnd);
	if(!fnd)
		return;

	add_word(keys, word);
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Reply
 *
 *	Purpose:	Generate a dictionary of reply words appropriate to the
 *			given dictionary of keywords.
 */
static DICTIONARY *reply(MODEL *model, DICTIONARY *keys)
{
	static DICTIONARY *replies = NULL;
	register int i;
	int symbol;
	bool start = TRUE;
	int basetime;

	Context;
	if(replies == NULL)
		replies = new_dictionary();
	free_dictionary(replies);

	/*
	 *	Start off by making sure that the model's context is empty.
	 */
	initialize_context(model);
	model->halcontext[0] = model->forward;
	used_key = FALSE;

	/*
	 *	Generate the reply in the forward direction.
	 */
	/* This used to be while(TRUE) and while it should never get stuck in an infinite loop in theory, this was changed just in case to timeout cause it can grab ram like crazy until it sigterms */
	basetime = time(NULL);
	while((time(NULL)-basetime) < timeout+2) {
		/*
		 *	Get a random symbol from the current context.
		 */
		if(start == TRUE)
			symbol = seed(model, keys);
		else
			symbol = babble(model, keys, replies);
		if((symbol==0) || (symbol==1))
			break;
		start = FALSE;

		/*
		 *	Append the symbol to the reply dictionary.
		 */
		replies->size += 1;
		if(realloc_dictionary(replies) == NULL) {
			error("reply", "Unable to reallocate dictionary");
			return NULL;
		}

		replies->entry[replies->size-1].length = model->dictionary->entry[symbol].length;
		replies->entry[replies->size-1].word = model->dictionary->entry[symbol].word;

		/*
		 *	Extend the current context of the model with the current symbol.
		 */
		update_context(model, symbol);
	}
	if((time(NULL)-basetime) >= timeout+2)
		putlog(LOG_MISC, "*", "TIMEOUT1!");


	/*
	 *	Start off by making sure that the model's context is empty.
	 */
	initialize_context(model);
	model->halcontext[0] = model->backward;

	/*
	 *	Re-create the context of the model from the current reply
	 *	dictionary so that we can generate backwards to reach the
	 *	beginning of the string.
	 */
	if(replies->size > 0)
		for(i=MIN(replies->size-1, model->order); i>=0; --i) {
			symbol = find_word(model->dictionary, replies->entry[i]);
			update_context(model, symbol);
		}

	/*
	 *	Generate the reply in the backward direction.
	 */
	basetime = time(NULL);
	while((time(NULL)-basetime) < timeout+2) {
		/*
		 *	Get a random symbol from the current context.
		 */
		symbol = babble(model, keys, replies);
		if((symbol==0) || (symbol==1))
			break;

		/*
		 *	Prepend the symbol to the reply dictionary.
		 */
		replies->size += 1;
		if(realloc_dictionary(replies) == NULL) {
			error("reply", "Unable to reallocate dictionary");
			return NULL;
		}

		/*
		 *	Shuffle everything up for the prepend.
		 */
		for(i=replies->size-1; i>0; --i) {
			replies->entry[i].length = replies->entry[i-1].length;
			replies->entry[i].word = replies->entry[i-1].word;
		}

		replies->entry[0].length = model->dictionary->entry[symbol].length;
		replies->entry[0].word = model->dictionary->entry[symbol].word;

		/*
		 *	Extend the current context of the model with the current symbol.
		 */
		update_context(model, symbol);
	}
	if((time(NULL)-basetime) >= timeout+2)
		putlog(LOG_MISC, "*", "TIMEOUT2!");

	return replies;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Evaluate_Reply
 *
 *	Purpose:	Measure the average surprise of keywords relative to the
 *			language model.
 */
static float evaluate_reply(MODEL *model, DICTIONARY *keys, DICTIONARY *words)
{
	register int i;
	register int j;
	int symbol;
	float probability;
	int count;
	float entropy = (float)0.0;
	TREE *node;
	int num = 0;
	bool fnd;

	Context;
	if(words->size <= 0)
		return (float)0.0;
	initialize_context(model);
	model->halcontext[0] = model->forward;
	for(i=0; i<words->size; ++i) {
		symbol = find_word(model->dictionary, words->entry[i]);

		// only calculate values for words in the reply that are also keywords in the original sentence
		search_dictionary(keys, words->entry[i], &fnd);
		if(fnd) {
			probability = (float)0.0;
			count = 0;
			++num;
			for(j=0; j<model->order; ++j)
				if(model->halcontext[j] != NULL) {
					node = find_symbol(model->halcontext[j], symbol);
					// the less that this word is used in this context, the higher the score
					// this is because we are dividing the amount of times the word is used in this context by the usage counter of the parent context
					if (surprise)
						probability += (float)(node->count)/(float)(model->halcontext[j]->usage);
					else
						probability += (float)((float)1.0-((node->count)/(float)(model->halcontext[j]->usage)));
					++count;
				}

			// log of <1 numbers are negative which is why we do -=
			// this will weigh the result according to the size of the context i think
			// in other words, the bigger the context, the higher the result value becomes
			if(count > 0.0)
				entropy -= (float)log(probability/(float)count);
		}

		update_context(model, symbol);
	}

	initialize_context(model);
	model->halcontext[0] = model->backward;
	for(i=words->size-1; i>=0; --i) {
		symbol = find_word(model->dictionary, words->entry[i]);

		search_dictionary(keys, words->entry[i], &fnd);
		if(fnd) {
			probability = (float)0.0;
			count = 0;
			++num;
			for(j=0; j<model->order; ++j)
				if(model->halcontext[j] != NULL) {
					node = find_symbol(model->halcontext[j], symbol);
					if (surprise)
						probability += (float)(node->count)/(float)(model->halcontext[j]->usage);
					else
						probability += (float)((float)1.0-((node->count)/(float)(model->halcontext[j]->usage)));
					++count;
				}

			if(count > 0.0)
				entropy -= (float)log(probability/(float)count);
		}

		update_context(model, symbol);
	}

	// hmm this looks like it helps to average out all sentences including long sentences with many keywords so that they are all comparable?
	if(num >= 8)
		entropy /= (float)sqrt(num-1);
	if(num >= 16)
		entropy /= (float)num;

	return entropy;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Make_Output
 *
 *	Purpose:	Generate a string from the dictionary of reply words.
 */
static wchar_t *make_output(DICTIONARY *words)
{
	static wchar_t *output = NULL;
	register int i;
	register int j;
	int length, tmp = 0;
	static wchar_t *output_none = NULL;

	Context;
	if(output_none == NULL)
		output_none = nmalloc(512);

	if(output == NULL) {
		output = (wchar_t *)nmalloc(sizeof(wchar_t));
		if(output == NULL) {
			error("make_output", "Unable to allocate output");
			return output_none;
		}
	}

	if(words->size == 0) {
		if(output_none != NULL)
			wcscpy(output_none, L"I am utterly speechless!");
		return output_none;
	}

	length = 1;
	for(i=0; i<words->size; ++i)
		length += (words->entry[i].length+1);

	output = (wchar_t *)nrealloc(output, sizeof(wchar_t)*length);
	if(output == NULL) {
		error("make_output", "Unable to reallocate output.");
		if(output_none != NULL)
			wcscpy(output_none, L"I forgot what I was going to say!");
		return output_none;
	}

	length = 0;
	for(i=0; i<words->size; ++i) {
		if (words->entry[i].word[0] == (wchar_t)31)
			tmp = 1;
		else tmp = 0;
		if (i>0 && (words->entry[i].word[0] != (wchar_t)31))
			output[length++]=L' ';
		for(j=0; j<words->entry[i].length-tmp; ++j)
			output[length++]=words->entry[i].word[j+tmp];
	}

	output[length]=L'\0';

	return output;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Babble
 *
 *	Purpose:	Return a random symbol from the current context, or a
 *			zero symbol identifier if we've reached either the
 *			start or end of the sentence.  Select the symbol based
 *			on probabilities, favouring keywords.  In all cases,
 *			use the longest available context to choose the symbol.
 */
static int babble(MODEL *model, DICTIONARY *keys, DICTIONARY *words)
{
	TREE *node = NULL;
	register int i;
	int count;
	int symbol = 0;
	bool fnd, fnd2;

	Context;
	/*
	 *	Select the longest available context.
	 */
	for(i=0; i<=model->order; ++i)
		if(model->halcontext[i] != NULL)
			node=model->halcontext[i];

	if(node->branch == 0)
		return 0;

	/*
	 *	Choose a symbol at random from this context.
	 */
	i = rnd(node->branch);
	count = rnd(node->usage);
	while(count >= 0) {
		/*
		 *	If the symbol occurs as a keyword, then use it.  Only use an
		 *	auxilliary keyword if a normal keyword has already been used.
		 */
		symbol = node->tree[i]->symbol;

		search_dictionary(keys, model->dictionary->entry[symbol], &fnd);
		search_dictionary(aux, model->dictionary->entry[symbol], &fnd2);
		if(fnd && ((used_key==TRUE) || !fnd2) && (word_exists(words, model->dictionary->entry[symbol])==FALSE)) {
			used_key = TRUE;
			break;
		}
		count -= node->tree[i]->count;
		i = (i >= (node->branch-1)) ? 0 : i+1;
	}

	return symbol;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Word_Exists
 *
 *	Purpose:	A silly brute-force searcher for the reply string.
 */
static bool word_exists(DICTIONARY *dictionary, STRING word)
{
	register int i;

	Context;
	for(i=0; i<dictionary->size; ++i)
		if(wordcmp(dictionary->entry[i], word) == 0)
			return TRUE;
	return FALSE;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Seed
 *
 *	Purpose:	Seed the reply by guaranteeing that it contains a
 *			keyword, if one exists.
 */
static int seed(MODEL *model, DICTIONARY *keys)
{
	register int i;
	int symbol;
	int stop;
	bool fnd;

	Context;
	/*
	 *	Fix, thanks to Mark Tarrabain
	 */
	if(model->halcontext[0]->branch == 0)
		symbol = 0;
	else
		symbol = model->halcontext[0]->tree[rnd(model->halcontext[0]->branch)]->symbol;

	if(keys->size>0) {
		i = rnd(keys->size);
		stop = i;
		while(TRUE) {
			search_dictionary(aux, keys->entry[i], &fnd);
			if((find_word(model->dictionary, keys->entry[i])!=0) && !fnd) {
				symbol = find_word(model->dictionary, keys->entry[i]);
				return symbol;
			}
			++i;
			if(i == keys->size)
				i=0;
			if(i == stop)
				return symbol;
		}
	}

	return symbol;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	New_Swap
 *
 *	Purpose:	Allocate a new swap structure.
 */
static SWAP *new_swap(void)
{
	SWAP *list;

	Context;
	list = (SWAP *)nmalloc(sizeof(SWAP));
	if(list == NULL) {
		error("new_swap", "Unable to allocate swap");
		return NULL;
	}
	list->size = 0;
	list->from = NULL;
	list->to = NULL;

	return list;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Add_Swap
 *
 *	Purpose:	Add a new entry to the swap structure.
 */
static void add_swap(SWAP *list, wchar_t *s, wchar_t *d)
{
	Context;
	list->size += 1;

	if(list->from == NULL) {
		list->from = (STRING *)nmalloc(sizeof(STRING));
		if(list->from == NULL) {
			error("add_swap", "Unable to allocate list->from");
			return;
		}
	}

	if(list->to == NULL) {
		list->to = (STRING *)nmalloc(sizeof(STRING));
		if(list->to == NULL) {
			error("add_swap", "Unable to allocate list->to");
			return;
		}
	}

	list->from = (STRING *)nrealloc(list->from, sizeof(STRING)*(list->size));
	if(list->from == NULL) {
		error("add_swap", "Unable to reallocate from");
		return;
	}

	list->to = (STRING *)realloc(list->to, sizeof(STRING)*(list->size));
	if(list->to == NULL) {
		error("add_swap", "Unable to reallocate to");
		return;
	}

	list->from[list->size-1].length = wcslen(s);
	list->from[list->size-1].word = mystrdup(s);
	list->to[list->size-1].length = wcslen(d);
	list->to[list->size-1].word = mystrdup(d);
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Initialize_Swap
 *
 *	Purpose:	Read a swap structure from a file.
 */
static SWAP *initialize_swap(char *filename)
{
	SWAP *list;
	FILE *file = NULL;
	char buffer[1024];
	char *from;
	char *to;

	Context;
	setlocale(LC_ALL, "");
	list = new_swap();

	if(filename == NULL)
		return list;

	file = fopen(filename, "r");
	if(file == NULL)
		return list;

	while(!feof(file)) {
		if(fgets(buffer, 1024, file)==NULL)
			break;
		if(buffer[0]=='#')
			continue;
		from = strtok(buffer, "\t ");
		to = strtok(NULL, "\t \n#");
		if (from && to) {
			wchar_t * wfrom = locale_to_wchar(from);
			wchar_t * wto = locale_to_wchar(to);
			add_swap(list, wfrom , wto);
		}
	}

	fclose(file);
	return list;
}

/*---------------------------------------------------------------------------*/

static void free_swap(SWAP *swap)
{
	register int i;

	Context;
	if(swap == NULL)
		return;

	for(i=0; i<swap->size; ++i) {
		free_word(swap->from[i]);
		free_word(swap->to[i]);
	}
	nfree(swap->from);
	nfree(swap->to);
	nfree(swap);
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Initialize_List
 *
 *	Purpose:	Read a dictionary from a file.
 */
static DICTIONARY *initialize_list(char *filename)
{
	DICTIONARY *list;
	FILE *file = NULL;
	STRING word = {0,NULL};
	char *string = NULL;
	wchar_t *wstring = NULL;
	char buffer[1024];

	Context;
	setlocale(LC_ALL, "");
	list = new_dictionary();

	if(filename == NULL)
		return list;

	file = fopen(filename, "r");
	if(file == NULL)
		return list;

	while(!feof(file)) {

		if(fgets(buffer, 1024, file) == NULL)
			break;
		if(buffer[0] == '#')
			continue;
		string = strtok(buffer, "\t \n#");
		if((string!=NULL) && (strlen(string)>0)) {
			wstring = locale_to_wchar(string);
			word.length = wcslen(wstring);
			word.word = locale_to_wchar(buffer);
			add_word(list, word);
			nfree(word.word);
			nfree(wstring);
		}
	}

	fclose(file);
	return list;
}

/*---------------------------------------------------------------------------*/

/*
 *	Function:	Rnd
 *
 *	Purpose:	Return a random integer between 0 and range-1.
 */
static int rnd(int range)
{
	static bool flag=FALSE;

	Context;
	if(flag==FALSE) {
		srand48(time(NULL));
	}
	flag = TRUE;
	return floor(drand48()*(double)(range));
}

/*---------------------------------------------------------------------------*/

static void load_personality(MODEL **model)
{
	FILE *file;
	char filename[512];
	char filename_train[512];
	bool btrain = FALSE;

	Context;
	setlocale(LC_ALL, "");

	/*
	 *	Check to see if the brain exists
	 */
	snprintf(filename, sizeof(filename), "%s%smegahal.brn", directory_cache, SEP);
	snprintf(filename_train, sizeof(filename_train), "%s%smegahal.trn", directory_resources, SEP);
	file = fopen(filename, "r");
	if(file == NULL) {
		file = fopen(filename_train, "r");
		btrain = TRUE;
		if(file == NULL) {
			error("load_personality", "Unable to allocate directory");
		}
	}

	if(!file) {
		error("load_personality", "Unable to find brain");
		return;
	}
	fclose(file);
	putlog(LOG_MISC, "*", "Changing to MegaHAL personality brains = \"%s\", train = \"%s\".\n", directory_cache, directory_resources);

	/*
	 *	Free the current personality
	 */
	free_model(*model);
	free_words(ban);
	free_dictionary(ban);
	free_words(aux);
	free_dictionary(aux);
	free_swap(swp);

	/*
	 *	Create a language model
	 */
	*model = new_model(order);

	/*
	 *	Train the model on a text if one exists
	 */
	if(btrain || load_model(filename, *model) == FALSE) {
		train(*model, filename_train);
	}

	/*
	 *	Read a dictionary containing banned keywords, auxiliary keywords,
	 *	greeting keywords and swap keywords
	 */
	snprintf(filename, sizeof(filename), "%s%smegahal.ban", directory_resources, SEP);
	ban = initialize_list(filename);
	snprintf(filename, sizeof(filename), "%s%smegahal.aux", directory_resources, SEP);
	aux = initialize_list(filename);
	snprintf(filename, sizeof(filename), "%s%smegahal.swp", directory_resources, SEP);
	swp = initialize_swap(filename);
}

/*---------------------------------------------------------------------------*/

static void change_personality(MODEL **model, const char * _directory_resources, const char * _directory_cache)
{
	if (_directory_resources) {
		snprintf(directory_resources, sizeof(directory_resources), "%s", _directory_resources);
	}

	if (_directory_cache) {
		snprintf(directory_cache, sizeof(directory_cache), "%s", _directory_cache);
	} else if (_directory_resources) {
		snprintf(directory_cache, sizeof(directory_cache), "%s%s%s", directory_resources, SEP, DIR_DEFAULT_CACHE);
	}

	load_personality(model);
}

/*---------------------------------------------------------------------------*/

static void free_words(DICTIONARY *words)
{
	register int i;

	Context;
	if(words == NULL)
		return;

	if(words->entry != NULL)
		for(i=0; i<words->size; ++i)
			free_word(words->entry[i]);
}

/*---------------------------------------------------------------------------*/

static void free_word(STRING word)
{
	Context;
	nfree(word.word);
}


/* Some personal notes by ^Baron^ about the inner structure of the brain (so that I dont have to break my head over the data structure again):
 * All words are stored in a single DICTIONARY struct
 * Words are sorted and constantly rearranged by reference in the index field (an array)
 * Words are added in the entry field which an array of strings and the symbol for the word is actually the position in this array
 * Inside the MODEL and TREEs, symbols are used, not words
 * DICTIONARY structs are used for storing other temporary or permanent lists of words
 * Note that while size starts with 1 for 1 entry, arrays start on 0, but the main model dictionary has 2 built in default words
 * The main dictionary is always in model->dictionary
 * TREE is the most commonly used struct and each instance represents a word
 * The word is stored as a symbol with usage and count counters (count=branch use count, usage=how many times all its subbranches are used)
 * Each TREE can have many branches which are stored in the tree field as an array of TREES and the branch field contains the size of this array
 * There is only one MODEL and it contains the main two TREE branches from which everything else is found
 * The halcontext stores temporary references to TREES while its building or learning a sentence etc and it is reset to NULL every time
 * The backward TREE is used to build a sentence backwards from a keyword
 * The order field is the max size of contiguous words that are stored in a branch and therefore also the max size of the halcontext array
 * A phrase is cut up into pieces (max = order) and stored in different branches starting with every single word in the phrase
 * In other words its something like this: 'grey dog and grey cat' will be stored twice in the branch beginning with 'grey' with two subranches as well as branches beginning with dog, and, and cat... and backwards in the backward main TREE
 * Phrases also use symbols and always must start with the size and end with a terminator (1) although when saved the terminator isnt saved
 */

/*
 *	$Id: megahal.c,v 1.25 1999/10/21 03:42:48 hutch Exp hutch $
 *
 *	File:		megahal.c
 *
 *	Program:	MegaHAL v8r6
 *
 *	Purpose:	To simulate a natural language conversation with a psychotic
 *			computer.  This is achieved by learning from the user's
 *			input using a third-order Markov model on the word level.
 *			Words are considered to be sequences of characters separated
 *			by whitespace and punctuation.  Replies are generated
 *			randomly based on a keyword, and they are scored using
 *			measures of surprise.
 *
 *	Author:		Mr. Jason L. Hutchens
 *
 *	WWW:		http://ciips.ee.uwa.edu.au/~hutch/hal/
 *
 *	E-Mail:		hutch@ciips.ee.uwa.edu.au
 *
 *	Contact:	The Centre for Intelligent Information Processing Systems
 *			Department of Electrical and Electronic Engineering
 *			The University of Western Australia
 *			AUSTRALIA 6907
 *
 *	Phone:		+61-8-9380-3856
 *
 *	Facsimile:	+61-8-9380-1168
 *
 *	Notes:		- This file is best viewed with tabstops set to three spaces.
 *			- To the Debian guys, yes, it's only one file, so shoot me!
 *			  I had to get it to work on DOS with crappy compilers and
 *			  I didn't want to spend more time than was neccessary.
 *			  Hence it's rather monolithic.  Also, an email would be
 *			  appreciated whenever bugs were fixed/discovered.  I've
 *			  terminated all of the memory leakage bugs AFAICT.  But
 *			  it does allocate a helluva lot of memory, I'll admit!
 *
 *	Compilation Notes
 *	=================
 *
 *	When compiling, be sure to link with the maths library so that the
 *	log() function can be found.
 *
 *	On the Macintosh, add the library SpeechLib to your project.  It is
 *	very important that you set the attributes to Import Weak.  You can
 *	do this by selecting the lib and then use Project Inspector from the
 *	Window menu.
 *
 *	CREDITS
 *	=======
 *
 *	Amiga (AmigaOS)
 *	---------------
 *	Dag Agren (dagren@ra.abo.fi)
 *
 *	DEC (OSF)
 *	---------
 *	Jason Hutchens (hutch@ciips.ee.uwa.edu.au)
 *
 *	Macintosh
 *	---------
 *	Paul Baxter (pbaxter@assistivetech.com)
 *	Doug Turner (dturner@best.com)
 *
 *	PC (Linux - Debian package)
 *	---------------------------
 *	Joey Hess (joeyh@master.debian.org)
 *
 *	PC (OS/2)
 *	---------
 *	Bjorn Karlowsky (?)
 *
 *	PC (Windows 3.11)
 *	-----------------
 *	Jim Crawford (pfister_@hotmail.com)
 *
 *	PC (Windows '95)
 *	----------------
 *	Jason Hutchens (hutch@ciips.ee.uwa.edu.au)
 *
 *	PPC (Linux)
 *	-----------
 *	Lucas Vergnettes (Lucasv@sdf.lonestar.org)
 *
 *	SGI (Irix)
 *	----------
 *	Jason Hutchens (hutch@ciips.ee.uwa.edu.au)
 *
 *	Sun (SunOS)
 *	-----------
 *	Jason Hutchens (hutch@ciips.ee.uwa.edu.au)
 */
/*===========================================================================*/

/*===========================================================================*/
/*
 *	$Log: megahal.c,v $
 *	Revision 1.25  1999/10/21 03:42:48  hutch
 *	Fixed problem on some operating systems caused by stderr and stdout not
 *	being of type FILE *.
 *
 *	Revision 1.24  1998/09/03  03:07:09  hutch
 *	Don't know.
 *
 *	Revision 1.23  1998/05/19 03:02:02  hutch
 *	Removed a small nmalloc() bug, and added a progress display for
 *	generate_reply().
 *
 *	Revision 1.22  1998/04/24 03:47:03  hutch
 *	Quick bug fix to get sunos version to work.
 *
 *	Revision 1.21  1998/04/24 03:39:51  hutch
 *	Added the BRAIN command, to allow user to change MegaHAL personalities
 *	on the fly.
 *
 *	Revision 1.20  1998/04/22 07:12:37  hutch
 *	A few small changes to get the DOS version to compile.
 *
 *	Revision 1.19  1998/04/21 10:10:56  hutch
 *	Fixed a few little errors.
 *
 *	Revision 1.18  1998/04/06 08:02:01  hutch
 *	Added debugging stuff, courtesy of Paul Baxter.
 *
 *	Revision 1.17  1998/04/02 01:34:20  hutch
 *	Added the help function and fixed a few errors.
 *
 *	Revision 1.16  1998/04/01 05:42:57  hutch
 *	Incorporated Mac code, including speech synthesis, and attempted
 *	to tidy up the code for multi-platform support.
 *
 *	Revision 1.15  1998/03/27 03:43:15  hutch
 *	Added AMIGA specific changes, thanks to Dag Agren.
 *
 *	Revision 1.14  1998/02/20 06:40:13  hutch
 *	Tidied up transcript file format.
 *
 *	Revision 1.13  1998/02/20 06:26:19  hutch
 *	Fixed random number generator and Seed() function (thanks to Mark
 *	Tarrabain), removed redundant code left over from the Loebner entry,
 *	prettied things up a little and destroyed several causes of memory
 *	leakage (although probably not all).
 *
 *	Revision 1.12  1998/02/04 02:55:11  hutch
 *	Fixed up memory allocation error which caused SunOS versions to crash.
 *
 *	Revision 1.11  1998/01/22 03:16:30  hutch
 *	Fixed several memory leaks, and the frustrating bug in the
 *	Write_Input routine.
 *
 *	Revision 1.10  1998/01/19 06:44:36  hutch
 *	Fixed MegaHAL to compile under Linux with a small patch credited
 *	to Joey Hess (joey@kitenet.net).  MegaHAL may now be included as
 *	part of the Debian Linux distribution.
 *
 *	Revision 1.9  1998/01/19 06:37:32  hutch
 *	Fixed a minor bug with end-of-sentence punctuation.
 *
 *	Revision 1.8  1997/12/24 03:17:01  hutch
 *	More bug fixes, and hopefully the final contest version!
 *
 *	Revision 1.7  1997/12/22  13:18:09  hutch
 *	A few more bug fixes, and non-repeating implemented.
 *
 *	Revision 1.6  1997/12/22 04:27:04  hutch
 *	A few minor bug fixes.
 *
 *	Revision 1.5  1997/12/15 04:35:59  hutch
 *	Final Loebner version!
 *
 *	Revision 1.4  1997/12/11 05:45:29  hutch
 *	The almost finished version.
 *
 *	Revision 1.3  1997/12/10 09:08:09  hutch
 *	Now Loebner complient (tm).
 *
 *	Revision 1.2  1997/12/08 06:22:32  hutch
 *	Tidied up.
 *
 *	Revision 1.1  1997/12/05  07:11:44  hutch
 *	Initial revision (lots of files were merged into one, RCS re-started)
 *
 *	Revision 1.7  1997/12/04 07:07:13  hutch
 *	Added load and save functions, and tidied up some code.
 *
 *	Revision 1.6  1997/12/02 08:34:47  hutch
 *	Added the ban, aux and swp functions.
 *
 *	Revision 1.5  1997/12/02 06:03:04  hutch
 *	Updated to use a special terminating symbol, and to store only
 *	branches of maximum depth, as they are the only ones used in
 *	the reply.
 *
 *	Revision 1.4  1997/10/28 09:23:12  hutch
 *	MegaHAL is babbling nicely, but without keywords.
 *
 *	Revision 1.3  1997/10/15  09:04:03  hutch
 *	MegaHAL can parrot back whatever the user says.
 *
 *	Revision 1.2  1997/07/21 04:03:28  hutch
 *	Fully working.
 *
 *	Revision 1.1  1997/07/15 01:55:25  hutch
 *	Initial revision.
 */
/*===========================================================================*/
