// Muhammad Riza Fairuzzabadi
// 21.01.2023

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Node
{
public:
    int depo_cap; // Depo Kapasitesi
    Node *left;   // Sol taraf
    Node *right;  // Sağ taraf
    Node()        // Constructor
    {
        depo_cap = 0; // Kapasite 0dan başlar
        left = NULL;  // Sol taraf NULL
        right = NULL; // Sağ taraf NULL
    }
};

class Antrepo // Antrepo Class, Doubly Linked List
{
public:
    Node *head;                                                              // F5, Linked List'in Başı
    Node *tail;                                                              // A1, Linked List'in Sonu
    Node *F4;                                                                // F4, 29 Geçiçi Depo ve Vagon, gelen Vagonlar buradan başlıyor
    Antrepo();                                                               // Constructor, varsayılan
    Antrepo(string BaslangicDurumu);                                         // Constructor, Başlangıç durumu varken
    ~Antrepo();                                                              // Destructor
    string AntrepoYerlestir(string depoDurum, vector<string> gelenVagonlar); // Istenen Method
    void Insert(Node *konveyor, int kargo);                                  // Geçici Depolara Vagondan yerleştirme, AntrepoYerleştir metodun içinde çağırılır.
    string Iterator();                                                       // En son depo durumuna bakmak için
};

Antrepo::Antrepo()
{
    for (int i = 0; i < 30; i++) // Constructor, varsayılan
    {
        Node *temp = new Node; // Yeni Node oluştur
        if (i == 0)            // Eğer ilk node ise, F5 olur
        {
            this->head = temp;
            this->tail = temp; // İlk node olması sebebiyle tail şu anlık F5
        }
        else
        {
            tail->right = temp; // Değilse, yeni node tail'in sağına eklenir
            temp->left = tail;  // Yeni node'nun solu da tail olur
            tail = temp;        // ve yeni node tail olur
            if (i == 1)         // F4 ikinci sırada olduğu için
                F4 = temp;
        }
    }
}

Antrepo::Antrepo(string def_cond) // Constructor, Başlangıç durumu varken
{
    def_cond.erase(def_cond.begin() + 1); // Onun durumuna bakmamıza gerek olmadığı için #'i sildik
    for (int i = 0; i < 30; i++)
    {
        Node *temp = new Node;
        temp->depo_cap = def_cond.at(i) - '0'; // yeni Node'un depo kasitesi, string'deki i karakteri int'e çevirerek alınır
        if (i == 0)                            // Eğer ilk node ise, F5 olur
        {
            this->head = temp;
            this->tail = temp; // İlk node olması sebebiyle tail şu anlık F5
        }
        else
        {
            tail->right = temp; // Değilse, yeni node tail'in sağına eklenir
            temp->left = tail;  // Yeni node'nun solu da tail olur
            tail = temp;        // ve yeni node tail olur
            if (i == 1)         // F4 ikinci sırada olduğu için
                F4 = temp;
        }
    }
}

Antrepo::~Antrepo()
{
    Node *temp = head; // head'den başlayarak
    while (temp != NULL)
    {
        Node *temp2 = temp->right; // onun sağındaki node'u temp2'ye atılır
        delete temp;               // temp silinir
        temp = temp2;              // temp te temp2'yi point eder ve o şekilde soldan sağa silinir while döngüsü içinde
    }
}

void Antrepo::Insert(Node *konveyor, int kargo)
{
    if (konveyor == head) // Eğer konveyor F5 ise, sonsuz olduğu için direkt olarak kargo eklenir
        konveyor->depo_cap += kargo;
    else
    {
        if (9 - konveyor->depo_cap >= kargo) // Eğer gelen kargo geçici depoya sığarsa, direkt olarak yerleştirilir
            konveyor->depo_cap += kargo;
        else
        {
            while (kargo > 0) // Eğer sığmazsa tek tek yerleştirilir bitene kadar
            {
                if (konveyor->depo_cap == 9) // Eğer geçici depo doluysa (==9), 1 kargo sol tarafa gönderilir.
                {
                    Insert(konveyor->left, 1);
                    konveyor->depo_cap = 0; // Geçici depo boşaltılır
                    kargo--;                // Cekicideki kargo sayısı 1 azaltılır
                }
                else
                { // geçici depo dolu değilse, kargo tek tek yerleştirilir
                    konveyor->depo_cap++;
                    kargo--;
                } // Bu kısım daha kısa da yazılır, ama istenenlere uygun yazılmasına özen gösterme niyetiyle böyle yazıldı.
            }
        }
    }
}

string Antrepo::Iterator() // En son depo durumuna bakmak 'çin
{
    string temp = "";                  // temp stringi oluşturulur,önce boş
    Node *iter = head;                 // F5'den başlar
    temp += to_string(iter->depo_cap); // F5 kapasitesi temp stringine eklenir
    temp += "#";                       // # eklenir
    if (iter->right != NULL)           // Eğer F5'in sağı boş değilse
        iter = iter->right;            // F4 olur
    while (iter != NULL)               // konveyor sonuna kadar
    {
        temp += to_string(iter->depo_cap); // her bir konveyorun depo kapasitesi temp stringine eklenir
        iter = iter->right;                // sonraki konveyora gecilir
    }
    delete iter; // iter silinir, heap'te yer kaplamaması için
    return temp; // temp stringi return edilir
}

string Antrepo::AntrepoYerlestir(string depoDurum, vector<string> gelenVagonlar)
{
    Node *temp = new Node; // temp node oluşturulur
    for (int i = 0; i < gelenVagonlar.size(); i++)
    {
        temp = F4;                          // temp F4'den başlar
        string gelen = gelenVagonlar.at(i); // gelenVagonlar vektöründen gelen vagonlar alınır
        for (int j = 0; j < 29; j++)
        {
            int kargo_int = gelen.at(j) - '0'; // gelen vagonun j. karakteri int'e çevrilir
            Insert(temp, kargo_int);           // vagondaki kargo kadar kargo yerleştirilir bu fonksiyonla
            temp = temp->right;                // soldan sağa
        }
        depoDurum = Iterator(); // Çekicideki bütün vagonlar bitince depo durumu güncellenir
    }
    delete temp;      // temp silinir
    return depoDurum; // depo durumu return edilir
}

int main()
{
    string depoDurum = "0#54134427902231984111412732221"; // Başlangıç durumu, programın olabildiğince kolay run edilebilmesi için user input olarak istenmedi şimdilik
    Antrepo *Dicle = new Antrepo(depoDurum);              // Antrepo oluşturulur, başlangıç durumu mevcut olduğundan ikinci constructor çağrılır
    vector<string> gelenVagonlar;                         // Gelen vagonlar vektörü oluşturulur
    string line;                                          // Dosyadan okunan satırlar line stringine atılır
    ifstream myfile("input.txt");                         // Dosya okunur, input.txt
    if (myfile.is_open())
    {
        while (getline(myfile, line))      // Dosya satır satır okunur ve line'a atılır
            gelenVagonlar.push_back(line); // line vektöre eklenir (arkasına)
        myfile.close();                    // Bitince dosya kapatılır
    }
    else
        cout << "Dosya acilamadi";                                 // Dosya açılamazsa hata mesajı verilir
    depoDurum = Dicle->AntrepoYerlestir(depoDurum, gelenVagonlar); // AntrepoYerlestir fonksiyonu çağrılır, depo durumu güncellenir
    cout << depoDurum << endl;                                     // Son durum ekrana yazdırılır
    return 0;                                                      // Program sonlandırılır
}