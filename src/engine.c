#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Value Value;
struct Value {
    float data;
    float grad;
    // backward function depends on how this node is created
    // for example, if it is created by Add, then backward function
    // will be:
    // Value1->grad += OutputValue->grad
    // Value2->grad += OutputValue->grad
    void (*backward)(Value*, Value*, Value*);
    Value* prevs[2];
    char op[10];
};

void print_value(Value* v)
{
    printf("v -> %p:\n", v);
    printf("\tv->data: %f\n", v->data);
    printf("\tv->grad: %f\n", v->grad);
    printf("\tv->backward: %p\n", v->backward);
    printf("\tv->prevs:\n");
    if(v->prevs[0] != NULL){
        printf("\t\tv[0]: %p\n", v->prevs[0]);
        printf("\t\t\tv[0]->data: %f\n", v->prevs[0]->data);
        printf("\t\t\tv[0]->grad: %f\n", v->prevs[0]->grad);
        printf("\t\t\tv[0]->op: %s\n", v->prevs[0]->op);
    }
    else if(v->prevs[1] != NULL){
        printf("\t\tv[1]: %p\n", v->prevs[1]);
        printf("\t\t\tv[1]->data: %f\n", v->prevs[1]->data);
        printf("\t\t\tv[1]->grad: %f\n", v->prevs[1]->grad);
        printf("\t\t\tv[1]->op: %s\n", v->prevs[1]->op);
    }
    else{
        printf("\t\tNone\n");
    }

    printf("\tv->op: %s\n", v->op);
};

void free_value(Value* v)
{
    v->backward = NULL;
    free(v);
};

void add_backward_function(Value* v1, Value* v2, Value* out)
{
    v1->grad += out->grad;
    v2->grad += out->grad;
};

Value* random_value()
{
    Value* v = (Value*)calloc(1, sizeof(Value));
    v->data = (float)(rand() / (float)(RAND_MAX)) * 10.0;
    v->prevs[0] = NULL;
    v->prevs[1] = NULL;
    strcpy(v->op, "None");

    return v;
};

Value* add_value(Value* v1, Value* v2)
{
    Value* v = (Value*)calloc(1, sizeof(Value));
    v->data = v1->data + v2->data;
    v->prevs[0] = v1;
    v->prevs[1] = v2;
    strcpy(v->op, "+");
    v->backward = add_backward_function;

    return v;
}

Value** random_values(int n)
{
    Value** vs = (Value**)calloc(n, sizeof(Value*));
    for(int i = 0; i < n; i++){
        vs[i] = random_value();
    }
    return vs;
}

int main(void)
{
    srand(time(NULL));

    Value* v1 = random_value();
    Value* v2 = random_value();

    Value* v3 = add_value(v1, v2);

    Value** vals = random_values(3);
    printf("========================\n");
    for(int i = 0; i < 3; i++){
        print_value(vals[i]);
    }
    printf("========================\n");

    v3->grad = 1.0;
    if(v3->backward != NULL){
        v3->backward(v1, v2, v3);
    }

    print_value(v1);
    print_value(v2);
    print_value(v3);

    free_value(v1);
    free_value(v2);
    free_value(v3);

    return 0;
}
