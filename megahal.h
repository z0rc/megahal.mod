/* See megahal.c comments */

/*===========================================================================*/

/*
 *  Copyright (C) 1998 Jason Hutchens
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
 *	$Id: megahal.h,v 1.3 1998/09/03 03:15:40 hutch Exp hutch $
 *
 *	File:		megahal.h
 *
 *	Program:	MegaHAL v8r5
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
 *	Notes:		This file is best viewed with tabstops set to three spaces.
 */

/*===========================================================================*/

#define _T(x) __T(x)
#define _TEXT(x) __T(x)
#define __T(x) L ## x

#define P_THINK 40
#define D_KEY 100000
#define V_KEY 50000
#define D_THINK 500000
#define V_THINK 250000

#define MIN(a,b) ((a)<(b))?(a):(b)

#define DIR_DEFAULT_RESOURCES "megahal.data/default"
#define DIR_DEFAULT_CACHE "brains"

#define COMMAND_SIZE (sizeof(command)/sizeof(command[0]))

#define BYTE1 uint8_t
#define BYTE2 uint16_t
#define BYTE4 uint32_t

#define SEP "/"

/*===========================================================================*/

#undef FALSE
#undef TRUE
typedef enum { FALSE, TRUE } bool;

typedef struct {
	BYTE1 length;
	wchar_t *word;
} STRING;

typedef struct {
	BYTE4 size;
	STRING *entry;
	BYTE2 *index;
} DICTIONARY;

typedef struct {
	BYTE2 size;
	STRING *from;
	STRING *to;
} SWAP;

typedef struct NODE {
	BYTE2 symbol;
	BYTE4 usage;
	BYTE2 count;
	BYTE2 branch;
	struct NODE **tree;
} TREE;

typedef struct {
	BYTE1 order;
	TREE *forward;
	TREE *backward;
	TREE **halcontext;
	BYTE4 phrasecount;
	BYTE2 **phrase;
	DICTIONARY *dictionary;
} MODEL;

typedef enum { UNKNOWN, QUIT, EXIT, SAVE, DELAY, HELP, SPEECH, VOICELIST, VOICE, BRAIN, PROGRESS, THINK } COMMAND_WORDS;

typedef struct {
	STRING word;
	wchar_t *helpstring;
	COMMAND_WORDS command;
} COMMAND;

/* megahal funcs */

static void add_aux(MODEL *, DICTIONARY *, STRING);
static void add_key(MODEL *, DICTIONARY *, STRING);
static void add_node(TREE *, TREE *, int);
static void add_swap(SWAP *, wchar_t *, wchar_t *);
static TREE *add_symbol(TREE *, BYTE2);
static BYTE2 add_word(DICTIONARY *, STRING);
static int babble(MODEL *, DICTIONARY *, DICTIONARY *);
static bool boundary(wchar_t *, int);
static void capitalize(wchar_t *);
static void change_personality(MODEL **, const char *, const char *);
static bool dissimilar(DICTIONARY *, DICTIONARY *);
static void error(char *, char *, ...);
static float evaluate_reply(MODEL *, DICTIONARY *, DICTIONARY *);
static TREE *find_symbol(TREE *, int);
static TREE *find_symbol_add(TREE *, int);
static BYTE2 find_word(DICTIONARY *, STRING);
static void free_dictionary(DICTIONARY *);
static void free_model(MODEL *);
static void free_tree(TREE *);
static void free_word(STRING);
static void free_words(DICTIONARY *);
static wchar_t *generate_reply(MODEL *, DICTIONARY *);
static void initialize_context(MODEL *);
static void initialize_dictionary(DICTIONARY *);
static DICTIONARY *initialize_list(char *);
static SWAP *initialize_swap(char *);
static void free_swap(SWAP *);
static void learn(MODEL *, DICTIONARY *);
static void load_dictionary(FILE *, DICTIONARY *);
static bool load_model(char *, MODEL *);
static void load_personality(MODEL **);
static void load_tree(FILE *, TREE *);
static void load_word(FILE *, DICTIONARY *);
static wchar_t *locale_to_wchar(char *);
static DICTIONARY *make_keywords(MODEL *, DICTIONARY *);
static wchar_t *make_output(DICTIONARY *);
static void make_words(wchar_t *, DICTIONARY *);
static DICTIONARY *new_dictionary(void);
static MODEL *new_model(int);
static TREE *new_node(void);
static SWAP *new_swap(void);
static DICTIONARY *reply(MODEL *, DICTIONARY *);
static void save_dictionary(FILE *, DICTIONARY *);
static void save_model(char *, MODEL *);
static void save_tree(FILE *, TREE *);
static void save_word(FILE *, STRING);
static int search_dictionary(DICTIONARY *, STRING, bool *);
static int search_node(TREE *, int, bool *);
static int seed(MODEL *, DICTIONARY *);
static void show_dictionary(DICTIONARY *);
static void train(MODEL *, char *);
static void update_context(MODEL *, int);
static void update_model(MODEL *, int);
static void upper(wchar_t *);
static bool warn(char *, char *, ...);
static char *wchar_to_locale(wchar_t *);
static int wordcmp(STRING, STRING);
static int wordcmp2(STRING, wchar_t *);
static bool word_exists(DICTIONARY *, STRING);

/* eggdrop funcs */

struct userrec; /* kill warnings */

static wchar_t *mystrdup(const wchar_t *);
static void mystrlwr(wchar_t *string);
static wchar_t* mynewsplit(wchar_t **);
static const wchar_t *mystrstr(const wchar_t *, const wchar_t *);
char *megahal_start();
static int megahal_expmem();
static int dictionary_expmem(DICTIONARY *);
static wchar_t *megahal_close();
static void megahal_report(int, int);
static bool floodcheck();
static char *istextinlist(char *, char *);
static char *istextinlist2(STRING, char *);
static int countchans();
static int getchannum(char *);
static void do_megahal(int, char *, char *, bool, char *, char *);
static int pub_megahal(char *, char *, char *, char *, char *);
static int pub_megahal2(char *, char *, char *, char *, char *);
static int pub_action(char *, char *, char *, char *, char *, char *);
static int dcc_megahal(struct userrec *, int, char *);
static int dcc_forget(struct userrec *, int, char *);
static int pub_forget(char *, char *, char *, char *, char *);
static int pub_forgetword(char *, char *, char *, char *, char *);
static int find_phrase(wchar_t *, bool *);
static void del_all_phrases(int);
static int dcc_megaver(struct userrec *, int, char *);
static int pub_megaver(char *, char *, char *, char *, char *);
static int recurse_tree(TREE *);
static void recurse_branch(TREE *);
static void decrement_tree(TREE *, TREE *);
static void trimdictionary();
static void recurse_tree_and_decrement_symbols(TREE *, int, int, int *);
static int tcl_treesize();
static int tcl_viewbranch();
static int tcl_trimbrain();
static int tcl_setmaxcontext();
static int tcl_reloadphrases();
static void reloadphrases();
static int tcl_learnfile();
static void del_phrase(int);
static int tcl_setmegabotnick();
static int tcl_savebrain();
static int tcl_reloadbrain();
static int tcl_learningmode();
static int tcl_talkfrequency();
static DICTIONARY *realloc_dictionary(DICTIONARY *);
static TREE *realloc_tree(TREE *);
static BYTE2 **realloc_phrase(MODEL *);
static void save_phrases(MODEL *);
static bool isrepeating(DICTIONARY *);
static bool isinprevs(DICTIONARY *);
static void updateprevs(wchar_t *);
static void strip_codes(wchar_t *);
static bool dissimilar2(DICTIONARY *, DICTIONARY *);
static int amount_bigger_than(int *, int, int);

/*===========================================================================*/

/*
 * $Log: megahal.h,v $
 * Revision 1.3  1998/09/03  03:15:40  hutch
 * Dunno.
 *
 *	Revision 1.2  1998/04/21 10:10:56  hutch
 *	Fixed a few little errors.
 *
 *	Revision 1.1  1998/04/06 08:02:01  hutch
 *	Initial revision
 */

/*===========================================================================*/
