#include "hash.h"
#include <math.h>


void hkdf_extract_sha256(const uchar_t* salt, size_t salt_len, const uchar_t* ikm, size_t ikm_len, uchar_t prk[32]) {

    if (!salt_len)
    {
        uchar_t buffer[SHA256_DIGEST_LENGTH] = {0} ; 

        hmac_sha256(buffer , SHA256_DIGEST_LENGTH , ikm , ikm_len , prk ) ; 
    } else {
        hmac_sha256(salt , salt_len , ikm , ikm_len , prk ) ; 

    }

}

// HKDF-Expand(PRK, info, L) -> OKM
void hkdf_expand_sha256(const uchar_t prk[32],
                       const uchar_t* info, size_t info_len,
                       uchar_t* okm, size_t okm_len) {
    size_t N = (size_t) ceil(okm_len / SHA256_DIGEST_LENGTH);
    uchar_t old_T[SHA256_DIGEST_LENGTH] ; 
    size_t len_T ; 
    uchar_t *T = malloc(sizeof(uchar_t)*okm_len) ;
    size_t inc = SHA256_DIGEST_LENGTH ; 
    size_t t = 0 ;
    size_t c = 0x1 ;

    for (size_t i = 1; i <= N; i++)
    {
        {
            uchar_t *info_padded  ;
            size_t info_padded_len  ;
            if (i==1)
            {
                info_padded_len = info_len  + 1 ;
                info_padded = malloc(sizeof(uchar_t)*(info_len  + 1)) ;

                memcpy(info_padded  , info , info_len ) ; 
            } else {
                info_padded_len = info_len + inc + 1 ;
                info_padded = malloc(sizeof(uchar_t)*(info_len + inc + 1)) ;
            
                memcpy(info_padded , T + (t-inc) , inc ) ; 
                memcpy(info_padded + inc , info , info_len ) ; 
            }
            
            info_padded[info_padded_len-1] = c ;
            c++ ; 
            hmac_sha256(prk , 32 ,info_padded  , info_padded_len , T + t ) ; 
            
        }
        
        t += inc ;

    }
    memcpy(okm , T  ,okm_len ) ; 

    free(T) ; 
}

void hkdf_sha256(const uchar_t* salt, size_t salt_len,
                const uchar_t* ikm, size_t ikm_len,
                const uchar_t* info, size_t info_len,
                uchar_t* okm, size_t okm_len) {
    uchar_t prk[32] ; 
    hkdf_extract_sha256(salt , salt_len , ikm , ikm_len , prk ) ; 
    hkdf_expand_sha256(prk , info , info_len , okm , okm_len) ; 
}

