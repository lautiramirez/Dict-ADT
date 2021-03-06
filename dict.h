#ifndef DICT_H
#define DICT_H

#include <stdio.h>
#include <stdbool.h>
#include "key_value.h"

typedef struct _node_t *dict_t;
/* dict is a pointer to a structure.
 *
 * Note: The definition of the structure should be in [dict.c], not here.
 * 
*/

dict_t dict_empty();
/* Creates an empty dictionary
 *
 *  PRE: {true}
 *
 *  USE: dict = dict_empty();
 *
 *  POS: {dict --> dict /\ dict_length(dict) == 0}
 * 
*/

dict_t dict_add(dict_t dict, key_t word, value_t def);
/* Adds a [word] and its definition [def] in the dictionary.
 * If [word] is already in the dictionary, its definition is replaced by [def].
 *
 *  PRE: {dict --> dict /\ word --> string_t /\ def --> string_t}
 *
 *  USE: dict = dict_add(dict, word, def);
 *
 *  POS: {dict --> dict /\ string_eq(def, dict_search(dict, word))}
 * 
*/

value_t dict_search(dict_t dict, key_t word);
/* Returns the definition of the given [word], or NULL if [word] is not in
 * the dictionary.
 *
 *  Note: Returns a reference to the string owned by the dictionary (not a copy).
 *
 *  PRE: {dict --> dict /\ word --> string_t}
 *
 *  USE: def = dict_search(dict, word);
 *
 *  POS: {(def==NULL /\ !dict_exists(dict, word)) \/ def != NULL}
 * 
*/

bool dict_exists(dict_t dict, key_t word);
/* Returns true if the given word exists in the dictionary, and false
 * otherwise.
 *
 *  PRE: {dict --> dict /\ word --> string_t}
 *
 *  USE: b = dict_exists(dict, word);
 *
 *  POS: {dict --> dict}
 * 
*/

unsigned int dict_length(dict_t dict);
/* Returns the number of words contained in the dictionary.
 * Complexity O(1)
 *
 *  PRE: {dict --> dict}
 *
 *  USE: n = dict_length(dict)
 *
 *  POS: {dict --> dict /\ n >= 0}
 * 
*/

dict_t dict_min(dict_t dict);
/* Finds and returns the alphabetically minor dict.
 *
 *  USE: dict = dict_min(dict);
 *
*/

dict_t dict_remove(dict_t dict, key_t word);
/* Removes the given word from the dictionary. If
 * [word] is not in the dictionary, it does nothing and
 * returns the dict unchanged.
 *
 *  PRE: {dict --> dict /\ word --> string_t}
 *
 *  USE: dict = dict_remove(dict, word);
 *
 *  POS: {dict --> dict /\ !dict_exists(dict, word)}
 * 
 */

dict_t dict_remove_all(dict_t dict);
/* Removes all the words from the dictionary.
 *
 *  PRE: {dict --> dict}
 *
 *  USE: dict = dict_remove_all(dict);
 *
 *  POS: {dict --> dict /\ dict_length(dict) == 0}
 * 
 */

void dict_dump(dict_t dict, FILE *file);
/* Prints all the words and its definitions in
 * the given file.
 *
 *  Note: Use [dict_dump(dict, stdout)] to print in the screen.
 *
 *  PRE: {dict --> dict /\ file != NULL}
 *
 *  USE: dict = dict_dump(dict, file);
 *
 *  POS: {dict --> dict}
 * 
 */

dict_t dict_destroy(dict_t dict);
/* Destroys the given dictionary, freeing all the allocated resources.
 *
 *  PRE: {dict --> dict}
 *
 *  USE: dict = dict_destroy(dict);
 *
 *  POS: {dict == NULL}
 * 
 */

#endif
