#include <assert.h>
#include <stdlib.h>

#include "dict.h"

struct _node_t {
    dict_t left;
    dict_t right;
    key_t key;
    value_t value;
};

dict_t dict_empty(){
    dict_t dict = NULL;
    assert(dict_length(dict) == 0);
    return dict;
}

dict_t dict_add(dict_t dict, key_t word, value_t def){
    assert(word != NULL && def != NULL);
    if(dict == NULL){
        dict = (dict_t)malloc(sizeof(struct _node_t));
        dict->key = word;
        dict->value = def;
        dict->left = NULL;
        dict->right = NULL;
    }
    else if(key_eq(dict->key, word)){  
        dict->value = value_destroy(dict->value);
        dict->value = def;
    }
    else if(key_less(word, dict->key)){
        dict->left = dict_add(dict->left, word, def);
    }
    else if(key_less(dict->key, word)){
        dict->right = dict_add(dict->right, word, def);
    }
    assert(value_eq(def, dict_search(dict, word)));
    return dict;
}

value_t dict_search(dict_t dict, key_t word){
    assert(word != NULL);
    key_t def=NULL;
    if(dict == NULL){
        def = NULL;
    }
    else if(key_eq(dict->key, word)){
        def = dict->value;
    }
    else if(key_less(word, dict->key)){
        def = dict_search(dict->left, word);
    }
    else if(key_less(dict->key, word)){
        def = dict_search(dict->right, word);
    }
    assert((def==NULL && !dict_exists(dict, word)) || def != NULL);
    return def;
}

bool dict_exists(dict_t dict, key_t word){
    assert(word != NULL);
    if(dict != NULL){
        if(key_eq(dict->key, word)){
            return true;
        }
        else if(key_less(word, dict->key)){
            return dict_exists(dict->left, word);
        } 
        else if(key_less(dict->key, word)){
            return dict_exists(dict->right, word);
        }
    }   
    return false;
}

unsigned int dict_length(dict_t dict){
    unsigned int n = 0;
    if(dict != NULL){
        n = n + 1;
        n = n + dict_length(dict->left);
        n = n + dict_length(dict->right);       
    }
    return n;
}

dict_t dict_min(dict_t dict){   
    while (dict->left != NULL){
        dict = dict->left;
    }
    return dict;
}

dict_t dict_remove(dict_t dict, key_t word){
    assert(word != NULL);
    if(dict == NULL){
        return dict;
    }
    else if(key_eq(dict->key, word)){      
        if(dict->left == NULL && dict->right == NULL){
            dict = dict_destroy(dict);
        } 
        else if(dict->left == NULL || dict->right == NULL){
            dict_t daux;
            if(dict->left != NULL){
                daux = dict->left;
            }
            else if(dict->right != NULL){
                daux = dict->right;
            }
            dict->key = key_destroy(dict->key);
            dict->value = value_destroy(dict->value);   
            free(dict);
            dict = daux;
        } 
        else if(dict->left != NULL && dict->right != NULL){
            
            /* To save the interface I don't use string_clone so I swap nodes */
            dict_t min = dict_min(dict->right);  

            /* CASE: if min's dad is root */
            if(key_eq(dict->right->key, min->key)){
                min->left = dict->left;
            } 

            else{

                /* Search to min's dad */
                dict_t dad_min = dict->right; 
                while(!key_eq(dad_min->left->key, min->key)){
                    dad_min = dad_min->left;
                }
                
                /* CASE: if min has not child unlink its father */
                if(min->right == NULL){
                    dad_min->left = NULL;
                }

                /* CASE: if min has child, link its father to its child (min's child) */
                else{
                    dad_min->left = min->right;
                }
                
                /* Link min whit dict's children */
                min->left = dict->left;
                min->right = dict->right;
            }

            /* Remove dict */
            dict->key = key_destroy(dict->key);
            dict->value = value_destroy(dict->value);
            dict->left = NULL;
            dict->right = NULL;
            free(dict);

            /* Link dict to min */
            dict = min;     
        }
    }
    else if(key_less(word, dict->key)){
        dict->left = dict_remove(dict->left, word);
    }
    else if(key_less(dict->key, word)){
        dict->right = dict_remove(dict->right, word);        
    }
    assert(!dict_exists(dict, word));
    return dict;
}

dict_t dict_remove_all(dict_t dict){
    if(dict != NULL){
        dict->left = dict_remove_all(dict->left);
        dict->right = dict_remove_all(dict->right);
        dict = dict_remove(dict, dict->key);
    }
    assert(dict_length(dict) == 0);
    return dict;
}

void dict_dump(dict_t dict, FILE *file){
    assert(file != NULL);
    if(dict != NULL){
        if(file == stdout){ 
            /* Shows the sorted dict in screen */
            dict_dump(dict->left, file);      
            key_dump(dict->key, file);
            fprintf(file, ": ");
            value_dump(dict->value, file);
            fprintf(file, "\n");
            dict_dump(dict->right, file);
        }
        else{ 
            /* Dump the dict but not sorted way(Shows the root, 
            then shows the left child that now is the root, and 
            repeat process then the shows the right child) */
            key_dump(dict->key, file);
            fprintf(file, ": ");
            value_dump(dict->value, file);
            fprintf(file, "\n");
            dict_dump(dict->left, file);      
            dict_dump(dict->right, file);
        }
    }
}

dict_t dict_destroy(dict_t dict){
    if(dict != NULL){     
        dict->key = key_destroy(dict->key);
        dict->value = value_destroy(dict->value);
        dict->left = dict_destroy(dict->left);
        dict->right = dict_destroy(dict->right);
        free(dict);
        dict = NULL;
    }
    assert(dict == NULL);
    return dict;
}