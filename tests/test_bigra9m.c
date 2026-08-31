#include <stdio.h>
#include "bigra9m.h"

int main()
{

    printf("hello from BIG AR9AM !!!!!!!!!!!!!!!!!!!\n") ; 
    BigRa9m a , b , c ; // or BigInt BigRa9m a , b , c ;  
    

    // make sure to initilize it first !!!
    // (use one of the 'inits' functions )
    bigra9m_inits(&a , &b , NULL) ;
    bigra9m_init_str(&c , "123456789987654321") ;
    
    
    // lets assign some HUGE numbers !!!
    bigra9m_assign_str(&a , "-123456876432654321351000650100000") ; 
    bigra9m_assign_uint64_t(&b , 0x123456789 ) ; 
    
    // now we print numbers to check 
    printf("a : ") ; 
    bigra9m_print(a) ; 
    printf("b : ") ; 
    bigra9m_print(b) ; 
    printf("c : ") ; 
    bigra9m_print(c) ;

    // do some operation on it 
    bigra9m_add(a , b , &c) ;
    

    // print results : 
    printf("a : ") ; 
    bigra9m_print(a) ; 
    printf("b : ") ; 
    bigra9m_print(b) ; 
    printf("c : ") ; 
    bigra9m_print(c) ; 

    // please clean your memory when you finish working !!!!!!
    bigra9m_clears(&a , &b , &c , NULL) ; 


}