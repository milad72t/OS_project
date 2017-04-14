//IPC_THREAD PROJECT 2 QUESTION 1
//milad teimouri
//91522059
//g++ -std=c++11 IPC-project2.cpp -o main -lpthread




#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <semaphore.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
using namespace std;

#define NUM_THREADS  1


pthread_t TPlus;
pthread_t TSub;
pthread_t TMul;
pthread_t TDiv;
pthread_t TWrite;

pthread_mutex_t mutexPlus;
pthread_mutex_t mutexSub;
pthread_mutex_t mutexMul;
pthread_mutex_t mutexDiv;
pthread_mutex_t mutexWrite;



struct Data {
	
	string result;
	int line;
	int op_pos;
	
	
};


Data BufferPlus[5];
Data BufferSub[5];
Data BufferMul[5];
Data BufferDiv[5];
Data BufferWrite[5];

sem_t FullPlus;
sem_t FullSub;
sem_t FullMul;
sem_t FullDiv;
sem_t FullWrite;

sem_t EmptyPlus;
sem_t EmptySub;
sem_t EmptyMul;
sem_t EmptyDiv;
sem_t EmptyWrite;


 int inPlus = 0;
 int outPlus = 0;
 
  int inSub = 0;
 int outSub = 0;
 
  int inDiv = 0;
 int outDiv = 0;
 
  int inMul = 0;
 int outMul = 0;
 
   int inWr = 0;
 int outWr = 0;

vector<Data> output; 



void *Read(void *t) {
	
	
	ifstream out("input.txt",ios::in);
	if (!out){
		cerr<<"ERROR\n";
	}
	out.seekg(0);
	int i;
	//int op;
	string s;
	int countLine=0;
	while(getline(out,s)){
		countLine++;
		int len=s.length();
		
		for (int k=0;k<len;k++){
			if (s[k]=='+'){
				
				//int result_=a+b;
				Data plus;
				plus.result=s;
				plus.line=countLine;
				plus.op_pos=k;
				//cout<<"detect +:"<<countLine<<endl;
				sem_wait(&EmptyPlus);
				pthread_mutex_lock(&mutexPlus);
				BufferPlus[inPlus] = plus;
				//cout<<"put buffer plus :"<<countLine<<endl;
				inPlus = (inPlus+1)%5;
				pthread_mutex_unlock(&mutexPlus);
				sem_post(&FullPlus);
				
				
				
				
				
				break;
			
				}
				if (s[k]=='-'){
				Data plus;
				plus.result=s;
				plus.line=countLine;
				plus.op_pos=k;
				//cout<<"detect -:"<<countLine<<endl;
				sem_wait(&EmptySub);
				pthread_mutex_lock(&mutexSub);
				BufferSub[inSub] = plus;
				//cout<<"put buffer sub :"<<countLine<<endl;
				inSub = (inSub+1)%5;
				pthread_mutex_unlock(&mutexSub);
				sem_post(&FullSub);
				
				
				break;
				}
				if (s[k]=='*'){
				Data plus;
				plus.result=s;
				plus.line=countLine;
				plus.op_pos=k;
				//cout<<"detect *:"<<countLine<<endl;
				sem_wait(&EmptyMul);
				pthread_mutex_lock(&mutexMul);
				BufferMul[inMul] = plus;
				//cout<<"put buffer Mul :"<<countLine<<endl;
				inMul = (inMul+1)%5;
				pthread_mutex_unlock(&mutexMul);
				sem_post(&FullMul);
				
				
				break;
				}
				if (s[k]=='/'){
				Data plus;
				plus.result=s;
				plus.line=countLine;
				plus.op_pos=k;
				//cout<<"detect /:"<<countLine<<endl;
				sem_wait(&EmptyDiv);
				pthread_mutex_lock(&mutexDiv);
				BufferDiv[inDiv] = plus;
				//cout<<"put buffer Div :"<<countLine<<endl;
				inDiv = (inDiv+1)%5;
				pthread_mutex_unlock(&mutexDiv);
				sem_post(&FullDiv);
				
				
				break;
			
				}
		
		}
		
		
		
	}

}


void *Write(void *t) {
	
	while(1){
		//usleep(10);
		sem_wait(&FullWrite);
		//cout<<"recive signal for writer"<<endl;
		pthread_mutex_lock(&mutexWrite);
		Data item = BufferWrite[outWr];
		
        outWr = (outWr+1)%5;
        pthread_mutex_unlock(&mutexWrite);
        //cout<<"read buffer writer :"<<outWr<<"line:"<<item.line<<endl;
        output.push_back(item);
		/*int line_=item.line;
		string result__=item.result;
		int res=atoi(result__.c_str());
		cout<<"writing line "<<item.line<<endl;
		ofstream writing;
		writing.open("output.txt", std::ios_base::app); 
		
		writing << line_<<"."<<res<<endl;

		writing.close();
		*/
		sem_post(&EmptyWrite);
		

	
	}
	
	
	
	

}


void *CalculatePlus(void *t) {
	
	
	while(1){
		
		sem_wait(&FullPlus);
		pthread_mutex_lock(&mutexPlus);
		Data item = BufferPlus[outPlus];
		string ss=item.result;
		int kk=item.op_pos;
		int a=atoi(ss.substr(0,kk).c_str());
		int b=atoi(ss.substr(kk+1,ss.length()-kk+1).c_str());
		int result_=a+b;
		//cout<<"recive singnal plus :"<<item.line<<endl;
		outPlus = (outPlus+1)%5;
		pthread_mutex_unlock(&mutexPlus);
		sem_post(&EmptyPlus);
        sem_wait(&EmptyWrite);
        pthread_mutex_lock(&mutexWrite);
        Data item2;
        item2.result=to_string(result_);
        item2.line=item.line;
        item2.op_pos=item.op_pos;
		BufferWrite[inWr]=item2;
		//cout<<"put to write buffer from plus"<<item.line<<endl;
		inWr = (inWr+1)%5;
		pthread_mutex_unlock(&mutexWrite);
		sem_post(&FullWrite);
		
		
	}

	

}





void *CalculateSub(void *t) {
	
	
	while(1){
		
		sem_wait(&FullSub);
		pthread_mutex_lock(&mutexSub);
		Data item = BufferSub[outSub];
		string ss=item.result;
		int kk=item.op_pos;
		int a=atoi(ss.substr(0,kk).c_str());
		int b=atoi(ss.substr(kk+1,ss.length()-kk+1).c_str());
		int result_=a-b;
		//cout<<"recive singnal sub :"<<item.line<<endl;
		outSub = (outSub+1)%5;
		pthread_mutex_unlock(&mutexSub);
		sem_post(&EmptySub);
        sem_wait(&EmptyWrite);
        pthread_mutex_lock(&mutexWrite);
        Data item2;
        item2.result=to_string(result_);
        item2.line=item.line;
        item2.op_pos=item.op_pos;
		BufferWrite[inWr]=item2;
		//cout<<"put to write buffer from sub"<<item.line<<endl;
		inWr = (inWr+1)%5;
		pthread_mutex_unlock(&mutexWrite);
		sem_post(&FullWrite);
		
		
	}

	

}





void *CalculateMul(void *t) {
	
	
	while(1){
		
		sem_wait(&FullMul);
		pthread_mutex_lock(&mutexMul);
		Data item = BufferMul[outMul];
		string ss=item.result;
		int kk=item.op_pos;
		int a=atoi(ss.substr(0,kk).c_str());
		int b=atoi(ss.substr(kk+1,ss.length()-kk+1).c_str());
		int result_=a*b;
		//cout<<"recive singnal mul :"<<item.line<<endl;
		outMul = (outMul+1)%5;
		pthread_mutex_unlock(&mutexMul);
		sem_post(&EmptyMul);
        sem_wait(&EmptyWrite);
        pthread_mutex_lock(&mutexWrite);
		Data item2;
        item2.result=to_string(result_);
        item2.line=item.line;
        item2.op_pos=item.op_pos;
		BufferWrite[inWr]=item2;
		//cout<<"put to write buffer from Mul"<<item.line<<endl;
		inWr = (inWr+1)%5;
		pthread_mutex_unlock(&mutexWrite);
		sem_post(&FullWrite);
		
		
	}

	

}





void *CalculateDiv(void *t) {
	
	
	while(1){
		
		sem_wait(&FullDiv);
		pthread_mutex_lock(&mutexDiv);
		Data item = BufferDiv[outDiv];
		string ss=item.result;
		int kk=item.op_pos;
		int a=atoi(ss.substr(0,kk).c_str());
		int b=atoi(ss.substr(kk+1,ss.length()-kk+1).c_str());
		int result_=a/b;
		//cout<<"recive singnal Div :"<<item.line<<endl;
		outDiv = (outDiv+1)%5;
		pthread_mutex_unlock(&mutexDiv);
		sem_post(&EmptyDiv);
        sem_wait(&EmptyWrite);
        pthread_mutex_lock(&mutexWrite);
		Data item2;
        item2.result=to_string(result_);
        item2.line=item.line;
        item2.op_pos=item.op_pos;
		BufferWrite[inWr]=item2;
		//cout<<"put to write buffer from Div"<<item.line<<endl;
		inWr = (inWr+1)%5;
		pthread_mutex_unlock(&mutexWrite);
		sem_post(&FullWrite);
		
		
	}

	

}


void SortOutput(){
	int i, j, flag = 1;    // set flag to 1 to start first pass
      Data temp;             // holding variable
      int numLength = output.size( ); 
      for(i = 1; (i <= numLength) && flag; i++)
     {
          flag = 0;
          for (j=0; j < (numLength -1); j++)
         {
         		Data tt1=output[j];
         		int tt=tt1.line;
         		Data bb1=output[j+1];
         		int bb=bb1.line;

               if (bb < tt)      
              { 
                    temp = output[j];          
                    output[j] = output[j+1];
                    output[j+1] = temp;
                    flag = 1;              
               }
          }
     }







}


void WriteOutput(){


for (int q=0;q<output.size();q++){
		Data item=output[q];
		string result__=item.result;
		int res=atoi(result__.c_str());
		//cout<<"writing line "<<item.line<<endl;
		ofstream writing;
		writing.open("output.txt", std::ios_base::app); 
		
		writing <<res<<endl;

		writing.close();




}









}





int main (int argc, char *argv[])
{	
	
	 sem_init(&FullPlus, 0, 0);
	 sem_init(&FullSub, 0, 0);
	 sem_init(&FullMul, 0, 0);
	  sem_init(&FullDiv, 0, 0);
	  sem_init(&FullWrite, 0, 0);
	  
	   sem_init(&EmptyPlus, 0, 5);
	 sem_init(&EmptySub, 0, 5);
	 sem_init(&EmptyMul, 0, 5);
	  sem_init(&EmptyDiv, 0, 5);
	  sem_init(&EmptyWrite, 0,5);

	  pthread_mutex_init(&mutexPlus, NULL);
	  pthread_mutex_init(&mutexSub, NULL);
	  pthread_mutex_init(&mutexDiv, NULL);
	  pthread_mutex_init(&mutexMul, NULL);
	  pthread_mutex_init(&mutexWrite, NULL);
	  
	
	  
	pthread_t Reader[NUM_THREADS];

	for(int ij = 0; ij < NUM_THREADS; ij++) {
      	pthread_create(&Reader[ij],NULL,Read,NULL);
    	}

   

	pthread_create(&TPlus,NULL,CalculatePlus,NULL);
	pthread_create(&TSub,NULL,CalculateSub,NULL);
	pthread_create(&TMul,NULL,CalculateMul,NULL);
	pthread_create(&TDiv,NULL,CalculateDiv,NULL);
	pthread_create(&TWrite,NULL,Write,NULL);
	

	sleep(1);
	
	for (int i=0; i<NUM_THREADS; i++) {
    	pthread_join(Reader[i], NULL);
  	}
  	//pthread_join(TPlus, NULL);
  	//pthread_join(TSub, NULL);
  	//pthread_join(TMul, NULL);
  	//pthread_join(TDiv, NULL);
  	//pthread_join(TWrite, NULL);
  	//sleep(10);
  	SortOutput();
  	WriteOutput();

  	//pthread_exit(NULL);

}
