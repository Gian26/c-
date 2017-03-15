// make && ./Neural_Network ../../wine2.data
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <stdlib.h>
#include <random>
#include <sstream>

using namespace std;

/*
   Variables
*/
vector<double> rows;
vector<vector<double> > ejemplos;

void read_data(char* argumento){

   string String;
   ifstream infile;
   infile.open(argumento);//argv[1]);
   int   count = 0;
   while(count <178){//!infile.eof()){
      count += 1;
      getline(infile,String);
      // cout<< count<<":  " <<String<< endl;

      stringstream  linestream(String);
      string cell;
      double numero = 0;
      rows.clear();
      while(getline(linestream,cell,',')){
         numero= strtod(cell.c_str(),NULL);
        //  cout<< count<<":  " <<cell<< endl;
         rows.push_back( numero );
      }
         ejemplos.push_back(rows);

      //este string lo mando para sacar los datos
   }
   infile.close();
//    cout << "FINISH"<<endl;
}

template <typename x>
void print_vector( x vector)
{
   for (int i = 0; i < vector.size(); ++i)
   {
      /* code */
      cout << vector[i]<< ", ";
   }
   cout << endl;
}

template <typename y>
void print_matrix(y &matriz){

   for (int i = 0; i < matriz.size(); ++i)
   {
      /* code */
      for (int j = 0; j < matriz[i].size(); ++j)
      {
         /* code */
         cout <<  matriz[i][j]<< " ";
      }
      cout << endl;
   }

}
double gen_rand() {
   std::random_device rd; // obtain a random number from hardware
   std::mt19937 eng(rd()); // seed the generator
   std::uniform_real_distribution<> distr(-0.05, 0.05); // define the range

   return  distr(eng); // generate numbers
}
int myrandom(int i){return rand()%i;}

double sigmoide(double z){

   return (1.0)/(1.0 + exp(-z));
}

vector<vector<double> > mat_max_min(vector<vector<double> > &mat){
vector<vector<double> > resultado;
   //vectorizar las columnas y sacar sus minimos y maximos
   for (int j = 1; j < mat[0].size(); ++j)
   {
      vector<double > row,temporal;
      for (int i = 0; i < mat.size(); ++i)
      {
         //sacar la columan
         temporal.push_back(mat[i][j]);
      }
      row.push_back( *min_element(temporal.begin(), temporal.end()) );
      row.push_back( *max_element(temporal.begin(), temporal.end()) );
      row.push_back(row[1] - row[0]);
      resultado.push_back(row);
      row.clear();
   }
   return resultado;
}

void Norm(vector<vector<double> > &matrix)
{
   vector< vector<double> > max_mins = mat_max_min(matrix);
//   vector< vector<double> > med_desv = obtain_std_mean(matrix);

   int inc = 0;
   //  normalizar
   for (int j = 1; j < matrix[0].size() ; ++j)
   {
      for (int i = 0; i < matrix.size(); ++i)
      {
         matrix[i][j]= (matrix[i][j] - max_mins[inc][0]) / max_mins[inc][2];
         //matrix[i][j] = (matrix[i][j] - med_desv[inc][0]) / med_desv[inc][1];
      }
      inc +=1;
   }

}

void partir_matriz(vector<vector<double> > &datos, int clases, vector< vector<vector<double> > > &v_salida)
{
   vector<vector<double> > v_x_i;
   int cont = 0,temp_i=0;
   while(cont < clases+1){
      for (int i = temp_i;i < datos.size(); ++i)
      {
         if(datos[i][0] == cont)
         {
            v_x_i.push_back(datos[i]);
         }
      }
      if (cont != 0){
         temp_i = v_x_i.size();
         v_salida.push_back(v_x_i);
         v_x_i.clear();
      }
      cont++;
   }
}

void merge_vectors(vector <vector<vector<double> > > &entrada, vector<vector<double> > &salida){
   int cont =0;
   while(cont < entrada.size()){
      salida.insert( salida.end(), entrada[cont].begin(), entrada[cont].end() );
      cont ++;
   }
}

void back_up_y_col(vector<vector<double> > &datos, vector<int> &y){
      for (int x = 0; x < datos.size(); x++)
      {
         y.push_back(datos[x][0]);
         // datos[x][0]=1;
         vector<double> temp(datos[x].begin()+1,datos[x].end());
         datos[x].clear();
         datos[x] = temp;
      }
}
   void class_dict(std::vector<std::vector<int> > &clase){
      vector<int> row;
      row.push_back(0);
      row.push_back(0);
      row.push_back(0);
      clase.push_back(row);
      row.clear();
      row.push_back(1);
      row.push_back(0);
      row.push_back(0);
      clase.push_back(row);
      row.clear();
      row.push_back(0);
      row.push_back(1);
      row.push_back(0);
      clase.push_back(row);
      row.clear();
      row.push_back(0);
      row.push_back(0);
      row.push_back(1);
      clase.push_back(row);
   }


struct Perceptron
{
   double sigma;
   vector<double> bias;
   vector<double> weight;
   vector<double> delta_w;
   vector<double> delta;
   vector<double> out;
   vector<double> net;

   void __init__(int v_size){
      for (int i = 0; i < v_size; ++i)
      {
         bias.push_back(gen_rand());
         weight.push_back(gen_rand());
         delta_w.push_back(0);
         delta.push_back(0);
      }
      out.push_back(0);
      net.push_back(0);
   }

   void updateDelta_W(int index,double element){
      delta_w[index] =  element;
   }

   void updateWeight(int index,double element){
      weight[index] =  element;
   }

   void updateDelta(int index,double element){
      delta[index] =  element;
   }

   void updateBias(int index,double element){
      bias[index] = element;
   }

   void updateOut(double o){
      out[0] = o;
   }
   void updateNet(double n){
      net[0] = n;
   }

   double getOut(){
      return out[0];
   }
   double getNet(){
      return net[0];
   }

   double getBias(int i){
      return bias[i];
   }

   double getDelta(int i){
      return delta[i];
   }
   double getWeight(int i){
      return weight[i];
   }
   double getDelta_W(int i){
      return delta_w[i];
   }

};

struct  layer
{
   vector<Perceptron> capa;

   void add_neuron(int v_size){
         Perceptron neurona;
         neurona.__init__(v_size);
         capa.push_back(neurona);
   }

   void print_neuron(){
     for (int i = 0; i < capa.size(); ++i)
      {  cout << " Weight ";
         for(int j = 0; j < capa[i].weight.size(); ++j){
            cout << capa[i].weight[j] << "  ";
         }
         cout << endl;
         cout << " Bias ";
         for(int j = 0; j < capa[i].bias.size(); ++j){
            cout << capa[i].bias[j] << "  ";
         }
         cout << endl;
         cout << " Delta ";
         for(int j = 0; j < capa[i].delta.size(); ++j){
            cout << capa[i].delta[j] << "  ";
         }
         cout << endl;
         cout << " NET ";
         cout << capa[i].net[0];

         cout << endl;
         cout << " OUT ";
         cout << capa[i].getOut()<< endl;
         cout << "  "<< endl;
      }

   }
   vector<double> getDelta(int neurona){
      return capa[neurona].delta;
   }
   vector<double> getPeso(int neurona){
      return capa[neurona].weight;
   }
   vector<double> getBias(int neurona){
      return capa[neurona].bias;
   }

   void reset_w(int neurona){
      for (int i = 0; i < capa.size(); ++i){
         for (int j = 0; j < capa[i].weight.size(); ++j)
         {
            capa[i].updateWeight(j,gen_rand());
            capa[i].updateBias(j,gen_rand());
            capa[i].updateDelta(j,0);
         }
         capa[i].updateNet(0);
         capa[i].updateOut(0);
      }
   }

};

struct network
{
   vector<layer> vector_capas;

   int n_capas;
    double eta;
    //eta = 0.15;

    network(){
        eta = 0.15;
    }
   void def_eta(double e){
      eta = e;
   }

   void add_layer(int n_neuronas, int v_size){
      layer capa;
      for (int i = 0; i < n_neuronas; ++i)
      {
         capa.add_neuron(v_size);
      }
      vector_capas.push_back(capa);
   }

   void get_size(int k){
      cout << vector_capas.size()<< endl;
   }

   void calculate_neuron_net(vector<double> &entrada,layer &capa){
      double sigma=0;
      // cout << " Calculando entradas: "<< endl;
      // for (int i = 0; i < entrada.size(); ++i)
      // {
      //    cout << entrada[i] << " ";
      // }
      // cout << endl;
      // cout << " "<< endl;
      //a recorrer cada nodo de esa capa y actualizar su vector de pesos
      for (int neurona = 0; neurona < capa.capa.size(); ++neurona)
         {
            // cout << " NEUORNA "<< neurona << endl;
            for (int bias = 0; bias < capa.capa[neurona].bias.size();bias ++){
               // cout << "Conexion: "<< bias << endl;
               for (int ent = 0; ent < entrada.size(); ++ent)
               {
                   sigma += entrada[ent]* capa.capa[neurona].weight[ent];
                   // cout << sigma << " += "<< entrada [ent] << " * "<< capa.capa[neurona].weight[ent]<< " "<< endl;
               }
               capa.capa[neurona].updateNet( sigma + capa.capa[neurona].bias[bias]);
               // cout<< "sumando al sigma el bias..."<< endl;
               // cout<< capa.capa[neurona].net[0] <<"  " << sigma + capa.capa[neurona].bias[bias]<<"  = "<< sigma <<" + "<< capa.capa[neurona].bias[bias]<< endl;
               // sigma = 0;
               // cout <<" " << endl;
            }
         }
    }

    void calculate_neuron_out(layer &capa){
      //calculando la salida
      // cout << " Calculando la salida de la neurona"<< endl;
         for (int neurona_i = 0; neurona_i < capa.capa.size(); ++neurona_i)
         {
            capa.capa[neurona_i].updateOut(sigmoide(capa.capa[neurona_i].getNet()) );
            // cout << " NET = "<< capa.capa[neurona_i].getNet()<< " out = "<< capa.capa[neurona_i].getOut()<< endl;
         }
    }

   void feed_forward(vector<double> &ejemplo){
      //primera entrada de 14 features
      // cout << "Example"<< endl;
      vector<double> entrada = ejemplo;
      // for (int i = 0; i < entrada.size(); ++i)
      // {
      //       cout << entrada[i]<< " ";
      // }
      // cout << endl;

      // cout << "feed_forward"<< endl;
      layer capa; // crea una capa temporal
      //vamos a propagar hacia adelnate actualizando los pesos y salidas
      for (int capak = 0; capak < vector_capas.size(); ++capak)
      {
         calculate_neuron_net(entrada, vector_capas[capak]);
         calculate_neuron_out( vector_capas[capak]);
         entrada.clear();

         // cout << "     "<< endl;
         for (int i = 0; i <  vector_capas[capak].capa.size(); ++i)
         {
            entrada.push_back( vector_capas[capak].capa[i].getOut());
         }
         // cout << "     "<< endl;
      }
   }

   void calcular_delta_neurona_salida(int sa ,vector<int> &clase){
      double delta=0.0,O=0.0;
      // cout<<" clase: "<<clase[0]<<" "<< clase[1]<<" "<< clase[2]<<endl;
      for (int neurona = 0; neurona < vector_capas[sa].capa.size(); ++neurona)
      {
         O = vector_capas[sa].capa[neurona].getOut();
         // cout<< "Neurona: "<<neurona <<" Salida: "<< O << " "<<endl;
          delta = O * (1- O)*(clase[neurona]-O);
          // cout << "delta: "<< delta <<endl;
         for (int i = 0; i < vector_capas[sa].capa[neurona].delta.size(); ++i)
         {
            // cout << "conexion "<< i << " "<< delta<<endl;
            vector_capas[sa].capa[neurona].updateDelta(i,delta);
         }
         // cout << " " << endl;

      }
   }

   void calcular_delta_neurona_oculta(int h){
      int nxt = h +1;
      //recorrer las neuronas de la capa
      for (int neurona = 0; neurona < vector_capas[h].capa.size(); ++neurona)
      {
         // cout <<" Capa " <<  h <<" Neurona " << neurona<< endl;
         double out = 0, temp= 0 ,sumatoria= 0;
         out  = vector_capas[h].capa[neurona].getOut();
         // cout << " Out: " << out << endl;
         // cout << " "<< endl;
         // vector_capas[nxt].print_neuron();
         for (int deltas = 0; deltas < vector_capas[h].capa[neurona].delta.size(); ++deltas)
         {
            // vector_capas[nxt].print_neuron();
            temp = out * (1-out);
            // cout <<" Conexiones "<< deltas <<endl;
            for (int i = 0; i < vector_capas[nxt].capa.size(); ++i)
            {
               // cout << " NXT neurona " <<i << endl;
               for (int j = 0; j < vector_capas[nxt].getDelta(i).size(); ++j)
                {
                   /* code */
                  double delta =  vector_capas[nxt].getDelta(i)[j];
                  double peso =  vector_capas[nxt].getPeso(i)[j];
                  // cout << " delta "<<delta<< "   Peso   " <<peso<< endl;
                  sumatoria +=  delta * peso;
                }
            }

            // cout << temp << " * " << sumatoria<< endl;
            temp *=(sumatoria);
            // cout << " delta "<< temp<< endl;;
            vector_capas[h].capa[neurona].updateDelta(deltas,temp);
            sumatoria = 0;
            // cout << " " << endl;
         }
//         temp = 0;
         // cout << " "<< endl;
      }
   }

  void feed_backward(int class_y){
      vector<vector<int> > clase;
      class_dict(clase);
      // cout << "feed_backward..."<< endl;
      calcular_delta_neurona_salida(vector_capas.size()-1,clase[class_y]);// actualizar la ultima capa
      // cout << " " << endl;

      for (int h = vector_capas.size()-2; h >= 0; --h)
         {
            // cout << " calcular deltas de capa oculta " << h << endl;
            calcular_delta_neurona_oculta(h);
         }
   }


   void actualizar_pesos(vector<double> ejemplo_xi){
      //tengo la capa y le voy a calcular sus delta_w
      // reverse(vector_capas.begin(),vector_capas.end());
      // cout << "actualizar pesos"<< endl;
      for (int capa = vector_capas.size()-1; capa >=0; --capa)
       {
         // cout << "Capa "<< capa<< endl;
         double anterior=0.0,deltaw,delta;
         for (int neurona = 0; neurona < vector_capas[capa].capa.size(); ++neurona)
         {
            if (capa == 0)
            {
               anterior = ejemplo_xi.size();
            }
            else{
               anterior = vector_capas[capa-1].capa.size();
            }
            // cout<<"Tamaño anterior" << anterior << endl;
            for (int i = 0; i < anterior; ++i)
            {
               // cout << "Neurona"<< i << endl;
               if (capa == 0)
               {
                  deltaw =  eta *  vector_capas[capa].capa[neurona].delta[i] *  ejemplo_xi[i];
                  // cout << eta << " * " <<   vector_capas[capa].capa[neurona].delta[i] << "*"  << ejemplo_xi[i]<< endl;

               }
               else{
                   deltaw =  eta *  vector_capas[capa].capa[neurona].delta[i] *  vector_capas[capa-1].capa[i].getOut();
                  // cout << eta << " * " <<   vector_capas[capa].capa[neurona].delta[i] << "*"  << vector_capas[capa-1].capa[i].getOut()<< endl;
               }
               // cout << "d= " <<deltaw << endl;
               // cout << "w= "<< vector_capas[capa].capa[neurona].weight[i]<< endl;
               delta = vector_capas[capa].capa[neurona].weight[i] + deltaw;
               vector_capas[capa].capa[neurona].updateWeight(i,delta);
               vector_capas[capa].capa[neurona].updateBias(i,(eta *  vector_capas[capa].capa[neurona].delta[i])+vector_capas[capa].capa[neurona].bias[i]);
            }
         }
       }
       // vector_capas[0].print_neuron();
       // vector_capas[1].print_neuron();
   }


   void reset_weights(){
      for (int i = 0; i < vector_capas.size(); ++i)
      {
         vector_capas[i].reset_w(i); // estar en capa
      }
   }
};

network crear_red(int n_hidden_capas, vector<int> &nneuronas){
   // backpropagation_network
   network backpropagation_network;
   for (int i = 0; i < n_hidden_capas; ++i)
   {
      backpropagation_network.add_layer(nneuronas[i+1],nneuronas[i]);
   }
   //creating net
   cout<< " creating network..."<< endl;
   backpropagation_network.add_layer(nneuronas[n_hidden_capas+1],nneuronas[n_hidden_capas]);
   // cout << "capa 1"<< endl;
   // backpropagation_network.vector_capas[0].print_neuron();
   // cout << "capa salida"<< endl;
   // backpropagation_network.vector_capas[1].print_neuron();
   return backpropagation_network;
}

void vectores_manual(vector<vector<vector<double> > > &entrada, vector<vector<double> > &salida){
   for (int i = 0; i < 30; ++i)
   {
      salida.push_back(entrada[0][i]);
      salida.push_back(entrada[1][i]);
      salida.push_back(entrada[2][i]);
   }
}

double calcular_error(layer &capa_salida,int salida_esperada){
   vector<vector<int> > clase;
   class_dict(clase);
   double error = 0.0;
   for (int i = 0; i < capa_salida.capa.size(); ++i)
    {
         error += pow(clase[salida_esperada][i] - capa_salida.capa[i].getOut(), 2);
    }
   return error;
}

double  sumar_vector(vector<double> &v)
{
   double suma = 0.0;
   for (int i = 0; i < v.size(); ++i)
   {
      suma+= v[i];
   }
   return suma/v.size();
}

int compare_out(vector<int> out, int esperada ){
   vector<vector<int> > clase;
   class_dict(clase);
   if (out[0]==clase[esperada][0] && out[1]==clase[esperada][1] && out[2]==clase[esperada][2])
   {
      return 1;
   }
   return 0;
}

void process(string* line) {
    cout << "line read: " << *line << endl;
    }


int main(int argc, char** argv) {
   srand ( unsigned ( std::time(0) ) );

   int clases = 3;
   int epocas = 1000;
   int muestra_clase=40;
   vector< vector<vector<double> > > v_salida;
   vector<vector<double> > v_normalizado;
   vector<vector<double> > ejemplos_test;
   vector<int> y_training;
   vector<int> y_test;
   vector<vector<double> > ejemplos_training;

   int no_capas_hidden = 1;
   int m = 0,r1=0,r2=9,incr=10,folds=0,eficiencia=0;
   vector<int> v_num_neurona_capa;
   v_num_neurona_capa.push_back(13);// tamaño de entrada
   v_num_neurona_capa.push_back(2); // capa de en medio
   v_num_neurona_capa.push_back(3); // capa de en medio
   v_num_neurona_capa.push_back(3); //capa de salida

    if (argc != 2){
      cerr << "provide one argument" << endl;
      return 1;
        }
        // string line;
        // cout << "* trying to open and read: " << argv[1] << endl;
        //
        // ifstream f (argv[1]);
        // // Note that we can omit checking for f.is_open(), because
        // // all errors will be catched correctly by f.fail() (!f) and
        // // we do not want to print error messages here.
        // // Also note that during the loop, the following rules are obeyed:
        // //   - first the IO operation, then error check, then data processing
        // //   - failbit and badbit prevent data processing, eofbit does not
        // while(getline(f, line)) {
        //     process(&line);
        //     }
        // f.close();


   read_data(argv[1]);


   //vector de ejemplos / numero de clases / vector de salida
   partir_matriz(ejemplos,clases,v_salida);

   for (int i = 0; i < clases; ++i)
   {
      v_salida[i].resize(muestra_clase);
   }

   merge_vectors(v_salida,v_normalizado);
   Norm(v_normalizado);
   v_salida.clear();
   partir_matriz(v_normalizado,clases,v_salida);

   //   vector de error
   vector<double> vector_error;
   vector<double> vector_epocas;
   double error;

   //crear red neuornal
   network backpropagation_network = crear_red(no_capas_hidden,v_num_neurona_capa);
   epocas = 10000;
   folds = 4;
   backpropagation_network.def_eta(0.2);
   cout<< "Eta: " << backpropagation_network.eta<< endl;
   cout<< " Ejecutar para "<< epocas<< " epocas "<< endl;

   // cross validation
   for (int j = 0; j < folds ; ++j){
         //cross validation
      cout<< " Validacion " << j << endl;
      cout<< " Seleccionar conjunto de entrenamiento y conjunto de prueba"<< endl;
      for (int ej = 0; ej <= muestra_clase-1; ++ej){
         // de r1 a r2 son los ejemplos de prueba, todo lo demas seran ejemplos
         // de entrenamiento
         if(ej >= r1 && ej <= r2){
            ejemplos_test.push_back(v_salida[0][ej]);
            ejemplos_test.push_back(v_salida[1][ej]);
            ejemplos_test.push_back(v_salida[2][ej]);
         }
         else{
            ejemplos_training.push_back(v_salida[0][ej]);
            ejemplos_training.push_back(v_salida[1][ej]);
            ejemplos_training.push_back(v_salida[2][ej]);
         }
      }
      random_shuffle ( ejemplos_training.begin(), ejemplos_training.end(),myrandom);
      random_shuffle (ejemplos_test.begin(),ejemplos_test.end(),myrandom);

      //sacar las 'y' para muestra y test.
      // guardar el tipo de clase del ejemplo en el vector de ys en orden
      back_up_y_col(ejemplos_training,y_training);
      back_up_y_col(ejemplos_test,y_test);

      //tamaño del vector de entrenamiento
      m = ejemplos_training.size();

      for (int iepoca = 0; iepoca < epocas; ++iepoca){
         //recorrer 90 ejemplos
         for (int i = 0; i < ejemplos_training.size(); ++i){
            backpropagation_network.feed_forward(ejemplos_training[i]);
            backpropagation_network.feed_backward(y_training[i]);//le paso la salida esperada
            backpropagation_network.actualizar_pesos(ejemplos_training[i]);
            error = calcular_error(backpropagation_network.vector_capas[backpropagation_network.vector_capas.size()-1],y_training[i]);
            vector_error.push_back(error);//mantener error de cada ejemplo
         }
         vector_epocas.push_back(sumar_vector(vector_error)*0.5); // cada elemento es un punto en la grafica.
      }

      for (int et = 0; et < ejemplos_test.size(); ++et){
         backpropagation_network.feed_forward(ejemplos_test[et]);
         int esperada = y_test[et];
         vector<int> output;

         for (int n = 0; n < backpropagation_network.vector_capas[backpropagation_network.vector_capas.size()-1].capa.size(); ++n){
            double out = backpropagation_network.vector_capas[backpropagation_network.vector_capas.size()-1].capa[n].getOut();
            if( out < 0.5){
               output.push_back(0);
            }
            else {
               output.push_back(1);
            }
         }
         if(compare_out(output,esperada))
            eficiencia += 1;
         output.clear();
      }

      cout<< " Asertados: " << (double)eficiencia/(double)ejemplos_test.size() <<endl;
      eficiencia = 0;
      //limpiamos los vectores para tomar otro conjunto de ejemplos
      ejemplos_test.clear();
      y_test.clear();
      vector_error.clear();
      ejemplos_training.clear();
      y_training.clear();
      // incrementar el rango de prueba
      r1 += incr;
      r2 += incr;

      //resetear pesos y graficar error
      cout<< " Resteando pesos"<< endl;
      cout <<" "<< endl;
      backpropagation_network.reset_weights();
   }

   std::ofstream ofs;
   ofs.open ("results.data", std::ofstream::out | std::ofstream::trunc);
   ofs << "training set 1 " << " training set 2 "<< " training set 3 " << " training set 4 "<< endl;
   for (int i = 0; i < epocas; ++i){
      ofs << i<< " " << vector_epocas[i] << " "<< vector_epocas[i+epocas]<< " "<< vector_epocas[i+epocas*2]<< " "<< vector_epocas[i+epocas*3]<< endl;
   }
   ofs.close();
   return 0;
}
