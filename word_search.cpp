#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream> //for stringstream
#include <string> //to_string
#include <vector>
using namespace std;

string most_accurate_str;
float best_accuracy =0;

void best_fit(int w);

void Keyword( string token, int word_cnt) {
   
   string line;
   string best_match;
   string updated_poem_file = "tmp_poem_file_"; //The file we read from
   string file_name = "tmp_poem_file_";  //The file we write to

   int words_matched =0;
   int words_total = 0;
   float ratio = 0;

   
   file_name.append(to_string(word_cnt));
   file_name.append(".txt");

   fstream matching_file;
   matching_file.open(file_name, std::ios_base::out);


   //When we are cheking the first word of a permutation
   if(word_cnt == 0){
      fstream poem_file("lepanto.txt");

      while (getline(poem_file, line)) {
         if (line.find(token) != string::npos) {
            matching_file << line << endl;
         }
      }
      
      matching_file.close();
      poem_file.close();
   }
   else //When checking the 2nd, 3rd...xth word, we read from a different file
   {
      updated_poem_file.append(to_string(word_cnt-1));
      updated_poem_file.append(".txt");
      fstream poem_file(updated_poem_file);

      while (getline(poem_file, line)) {
         if (line.find(token) != string::npos) {
            matching_file << line << endl;
         }
      }

      matching_file.close();
      poem_file.close();     
   }  
   
}

void permute(vector<string> a, int l, int r, int total)
{
    string new_permutation;
    string token;
    int word_count = 0;

    if (l == r){
      //Turn the new input permutation to a string
      for(int i = total; i >0 ;i--){
         new_permutation.append(a[i-1]);
         new_permutation = new_permutation+" ";
      }   
      //cout <<"New: "<<  new_permutation << endl;
      
      //Take each input permutation and search the poem with it
      stringstream token_list(new_permutation);
      while(token_list >> token){
         Keyword(token, word_count++);
      }
      best_fit(word_count);
   }
    else
    {
        // Permutations made
        for (int i = l; i <= r; i++)
        {
            // Swapping done
            swap(a[l], a[i]);
 
            // Recursion called
            permute(a, l+1, r, total);
 
            //backtrack
            swap(a[l], a[i]);
        }
    }
}

void best_fit(int word_count){
   float accuracy = 0;
   int delete_files = word_count; // the max tmp files created
   string best_fit_test;
   string tmp_token;

   while(word_count >= 0){
      string file_name = "tmp_poem_file_";
      file_name.append(to_string(word_count-1));
      file_name.append(".txt");
      fstream final_file(file_name);         

      //cout << file_name << " reading from \n";
      getline(final_file,best_fit_test);
      if(best_fit_test == "\0"){
         word_count--;
      }     
      else
      {
         //count the number of words on the line, to calc accuracy
         stringstream words_in_line(best_fit_test);
         while (words_in_line>>tmp_token)
         {
            accuracy++;
         }

         //cout << "Accuracy:" << word_count << " / " << accuracy <<  " = " << word_count / accuracy <<  endl;
         accuracy = (word_count / accuracy);
         break;
      }   
   }

   //Swap the global "most_accurate_str" if a better one is found
   if(accuracy > best_accuracy){
      most_accurate_str = best_fit_test;
      best_accuracy = accuracy;
   }

   //Delete all the tmp files created
   for(;delete_files >= 0; delete_files-- ){
      string file_name = "tmp_poem_file_";
      file_name.append(to_string(delete_files));
      file_name.append(".txt");
      const char* file_to_remove = file_name.c_str();
      remove(file_to_remove);
   }
}

int main(){

   string input; 
   string token; 
   
   vector <string> permutation;

   int token_combonations =0;

  
   cout << "Hello, please enter the words you remember\n";
   getline(cin,input);
   stringstream token_count(input);

   while (token_count >> token)
   {
      token_combonations++;
      permutation.push_back(token);      //Adds each search word to a vactor
   }

   permute(permutation, 0, token_combonations-1, token_combonations);
   
   if (best_accuracy == 0)
   {
      cout << "\nSorry a problem occured, please try different search terms.\n"; 
   }
   else
   {
         cout << "\nThe most likely line is:\n" << most_accurate_str << endl;
         //cout << "With and accuracy of: " << best_accuracy <<endl;
   }
   
   
}