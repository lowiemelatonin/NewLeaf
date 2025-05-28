#include "ast.h"
#include "hashmap.h"
#include "eval.h"

int main(){
    HashMap *env = create_map(8);
    free_map(env);

    return 0;
}