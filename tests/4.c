int main(){
    // Comentario em linha
    printf("Hello, world!\n");

    float f = 0.5;
    _Bool b = 1;

    if(b== 1 && (f!= 0.0 || f <=1.5)){
        for(int i=0; i<100; i++){
            if(f >=0){
                f= f - 2;
            }
            if(f > 0){
                f--;
            }
            if(f < 0){
                f = f+2;
            }
            f = f + 1;
        }
    }

    /*
        Comentario em bloco



    */

    float x = f * 5 / 20;
}