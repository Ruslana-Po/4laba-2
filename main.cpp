#include "haeder.h"

int main() {
    cout<<"Выберете\n 1 - однин поток\n 2 - несколько\n";
    int number;
    cin>>number;
    if(number==1){
         One();
    }else if(number ==2){
        two();
    }
  
    return 0;
}
