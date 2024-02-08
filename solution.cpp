#include <vector>
#include <sstream>
#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <math.h>
using namespace std;
 
ofstream out("iris-flowers.txt", std::ios::app);
int N; //количество строк в файле

//функция сортировки массива
void Sort(vector <float> &array, int size, int start, int finish)
{
    int min=0, i, j;
    float temp;
    for (i=start; i<finish; i++)
    {
        min=i;
        for(j=i+1; j<finish; j++)
             if (array[j] < array[min])
                min = j;
        temp =  array[i];
        array[i] =  array[min];
        array[min] = temp;
    }
}

//функция нахождения среднего арифметического
float mean (vector <float> &array)
{
    int i=0;
    float sum=0;
    for (i=0; i<array.size(); i++)
        sum+=array[i];
   
    return sum/array.size();
}

//функция, которая находит среднеквадратичное отклонение
float stan_dev (vector <float> &array)
{
    int i=0;
    float M_Iris_setosa[array.size()][2];
    for(i=0; i<array.size(); i++)
        M_Iris_setosa[i][1]=1;
    
    float p=-1;
    int j=-1;
    for(i=0; i<array.size(); i++)
    { 
        if(array[i]==p)
        {
            M_Iris_setosa[j][0]=array[i];
            M_Iris_setosa[j][1]+=1;
        }
        else
        {  
            j++;
            M_Iris_setosa[j][0]=array[i];
            p=array[i];
        }
    } 
    for(i=0; i<=j; i++)
        M_Iris_setosa[i][1]/=array.size();
    
    float M=0, D=0; //M - математическое ожидание, D - среднеквадратичное
    for (i=0; i<=j; i++)
    {
        M += M_Iris_setosa[i][0]*M_Iris_setosa[i][1];
        D+=M_Iris_setosa[i][0]*M_Iris_setosa[i][0]*M_Iris_setosa[i][1];
    }
    D-=M*M;
    D*=(float)array.size()/(array.size()-1); //несмещенная дисперсия
    return sqrt(D);
}

//функция, которая ищет количество чисел, лежащих в заданном промежутке
int num_in_interval(vector <float> &array, float left_bound, float right_bound)
{
    int i;
    int num=0;
    for (i=0; i<array.size(); i++)
        if( array[i] >=left_bound && array[i] < right_bound)
            num++;
    return num;
}

int main()
{ 
    string str;
    vector<string> vec;
    ifstream inf("iris.data");
    int i=0, j=0;
    
    while (getline(inf, str))
    {
        stringstream ss(str);
        while (getline(ss, str, ','))
        {
            vec.push_back(str);
        }
    }
    N=vec.size()/5;
    
    vector<string> names;
    vector<int> count;
   
    /*names.push_back(vec[4]);
    for(i=9; i<vec.size(); i+=5)
    { 
        if(names[j]!=vec[i])
        {
            count.push_back(i);
            j++;
            names.push_back(vec[i]);
        }
   }*/

    //создаем массивы векторов, чтобы записать туда длину и ширину лепестков
    vector<float> data_length[4]; //использую вектор, чтобы не выделять лишннюю память, у меня будет 3 элемента по 50 и один по 150
    vector<float> data_width[4];
    
   for (i=0; i<N; i++){
        if(i<N/3)
        {
           data_length[0].push_back(stof(vec[i*5]));
           data_length[3].push_back(stof(vec[i*5]));
           
           data_width[0].push_back(stof(vec[i*5+1]));
           data_width[3].push_back(stof(vec[i*5+1]));
        }
        if(i>=N/3 && i<N/3*2)
        {
           data_length[1].push_back(stof(vec[i*5]));
           data_length[3].push_back(stof(vec[i*5]));
           
           data_width[1].push_back(stof(vec[i*5+1]));
           data_width[3].push_back(stof(vec[i*5+1]));
        }
        if(i>=N/3*2)
        {
           data_length[2].push_back(stof(vec[i*5]));
           data_length[3].push_back(stof(vec[i*5]));
           
           data_width[2].push_back(stof(vec[i*5+1]));
           data_width[3].push_back(stof(vec[i*5+1]));
        }
   }
 
    //for (i=0; i<50; i++)
      //  cout << data_width[2][i] << endl;
   //сортируем данные в массивах
    for (i=0; i<4; i++)
    {
        Sort(data_width[i], N/3, 0, data_width[i].size());
        Sort(data_length[i], N/3, 0, data_length[i].size());
    }
    char array[26][130]; //матрица, в которой посимвольно хранятся данные
    
   string param[26]={"SEPAL LENGTH [CM]", "N", "MEAN", "MIN", "MEDIAN", "MAX", "STANDARD DEVIATION", "SEPAL LENGTH (%)", "< 5", ">=5 AND <6", ">=6 AND <7", ">= 7", "SEPAL WIDTH [CM]", "N", "MEAN", "MIN", "MEDIAN", "MAX", "STANDARD DEVIATION", "SEPAL WIDTH (%)", "< 3", ">=3 AND <3.5", ">=3.5"};
   string name_col[4]={"Iris Setosa", "Iris Versicolor", "Iris Virginica", "Total"};
   
    int W=130; //ширина области, в которой будут выведены данные
    for (i=0; i<26; i++)
      for (j=0; j<W; j++)
            array[i][j]=' ';

    string str_name="Iris Flower Summary";
    for (j=0; j<str_name.length(); j++)
        array[0][(W-str_name.length())/2+j]=str_name[j];
    
    str_name="All Flowers";
    for (j=0; j<str_name.length(); j++)
        array[1][(W-str_name.length())/2+j]=str_name[j];

    int h=26;
    for (i=0; i<4; i++)
    {
        for (j=0; j<name_col[i].length(); j++)
            array[2][j+h]=name_col[i][j];
        h+=26;
    }

    for (i=3; i<26; i++)
        for (j=0; j<param[i-3].length(); j++)
               array[i][j]=param[i-3][j];

    int  k=26, index=1;
    int MAX=10000; //заведомо большое число
    string strin;
    
    for (i=0; i<4; i++)
    {
        index=4;
        stringstream ss;
        
        //ДЛЯ ДЛИНЫ
         ss << fixed << data_length[i].size();
        strin = ss.str();
         for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str("");  
        index++;
        
        //среднее арифметическое
       // ss << fixed << setprecision(2) << mean(sep_length, 50, st, fin);
        ss << fixed << setprecision(2) << mean(data_length[i]);
        strin = ss.str();
         for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str(""); 
        index++;
        
        //минимальное в отсортированном массиве
        ss << fixed << setprecision(1) << data_length[i][0];
        
        strin = ss.str();
         for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str(""); 
        index++;
         
        //медиана в отсортированном массиве   
        ss << fixed << setprecision(2) << (data_length[i][data_length[i].size()/2-1]+data_length[i][data_length[i].size()/2])/2;
       // cout << "\n" <<  data_length[i][24] << " " << data_length[i][25] << "\n";
        strin = ss.str();
        for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str(""); 
        index++;
      
        //максимальное в отсортированном массиве
        ss << fixed << setprecision(1) << data_length[i][data_length[i].size()-1];
        strin = ss.str();
        for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str(""); 
        index++;
        
        
        //среднеквадратичное
        ss << fixed << setprecision(2) << stan_dev(data_length[i]);
         strin = ss.str();
        for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str(""); 
        index+=2;
        
        //<5
        ss << num_in_interval(data_length[i], 0, 5);
        strin = ss.str();
         strin = strin + ' ' + '(';
        ss.str("");
        ss << setprecision(1) << (float) num_in_interval(data_length[i], 0, 5)/data_length[i].size()*100;
        strin = strin + ss.str();
        strin = strin + ')';
        for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str("");
        index++;
        
        //>=5 AND <6 
        ss << num_in_interval(data_length[i], 5, 6);
        strin = ss.str();
        strin = strin + ' ' + '(';
        ss.str("");
        ss << setprecision(1) << (float) num_in_interval(data_length[i], 5, 6)/data_length[i].size()*100;
        strin = strin + ss.str();
        strin = strin + ')';
        for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str("");
        index++;
        
        //>=6 AND <7
        ss << num_in_interval(data_length[i], 6, 7);
        strin = ss.str();
        strin = strin + ' ' + '(';
        ss.str("");
        ss << setprecision(1) << (float) num_in_interval(data_length[i], 6, 7)/data_length[i].size()*100;
        strin = strin + ss.str();
        strin = strin + ')';
        for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str("");
        index++;
        
        //>= 7  
        ss << num_in_interval(data_length[i], 7, MAX);
        strin = ss.str();
        strin = strin + ' ' + '(';
        ss.str("");
        ss << setprecision(1) << (float) num_in_interval(data_length[i], 7, MAX)/data_length[i].size()*100;
        strin = strin + ss.str();
        strin = strin + ')';
        for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str("");
        index+=2;
        
        //ДЛЯ ШИРИНЫ
         //среднее арифметическое
         
        ss << fixed << data_width[i].size();
        strin = ss.str();
         for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str("");  
        index++;
        
        ss << fixed << setprecision(2) << mean(data_width[i]);
        strin = ss.str();
         for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str("");  
        index++;
        
         //минимальное в отсортированном массиве
        ss << fixed << setprecision(1) << data_width[i][0];
        strin = ss.str();
         for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str("");  
        index++;
        
        //медиана в отсортированном массиве   
        ss << fixed << setprecision(2) << (data_width[i][data_width[i].size()/2-1]+data_width[i][data_width[i].size()/2])/2;
        strin = ss.str();
        for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str("");
        index++;
      
        //максимальное в отсортированном массиве
        ss << fixed << setprecision(1) << data_width[i][data_width[i].size()-1];
        strin = ss.str();
        for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str(""); 
        index++;
        
        //среднеквадратичное
        ss << fixed << setprecision(2) << stan_dev(data_width[i]);
         strin = ss.str();
        for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str(""); 
        index+=2;
        
        //<3
        ss << num_in_interval(data_width[i], 0, 3);
        strin = ss.str();
         strin = strin + ' ' + '(';
        ss.str("");
        ss << setprecision(1) << (float) num_in_interval(data_width[i], 0, 3)/data_length[i].size()*100;
        strin = strin + ss.str();
        strin = strin + ')';
        //cout << num_in_interval(sep_length, 50, st, fin, 0, 3) << "   ";
        for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str("");
        index++;
        
         //>=3 AND <3.5 
        ss << num_in_interval(data_width[i], 3, 3.5);
        strin = ss.str();
         strin = strin + ' ' + '(';
        ss.str("");
        ss << setprecision(1) << (float) num_in_interval(data_width[i], 3, 3.5)/data_length[i].size()*100;
        strin = strin + ss.str();
        strin = strin + ')';
        //cout << num_in_interval(sep_length, 50, st, fin, 0, 3) << "   ";
        for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str("");
        index++;
        
         //>=3.5
        ss << num_in_interval(data_width[i], 3.5, MAX);
        strin = ss.str();
        strin = strin + ' ' + '(';
        ss.str("");
        ss << setprecision(1) << (float) num_in_interval(data_width[i], 3.5, MAX)/data_length[i].size()*100;
        strin = strin + ss.str();
        strin = strin + ')';
        //cout << num_in_interval(sep_length, 50, st, fin, 0, 3) << "   ";
        for (j=0; j<strin.length(); j++)
            array[index][k+j] = strin[j]; 
        ss.str("");
        index++;
        
        k+=26;
    }
    for (i=0; i<26; i++)
    {
        for (j=0; j<130; j++)
            out << array[i][j];
        out << "\n";
    }
    return 0;
}
