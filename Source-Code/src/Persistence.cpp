#include "Persistence.h"

Persistence::Persistence(string file)
{
    this->file = file;
}

Persistence::~Persistence()
{
    //dtor
}

bool Persistence::Retrieve(Generation &generation)
{
    cout << "Loading data from " << file << endl;

    ifstream input;
    input.open(file.c_str());
    if(input.fail()) {
        cerr << "Error opening file." << endl;
        return false;
    }

    generation.Reset();

    char line;
    while(input) {
        string s;
        getline(input, s);
        line = s[0];

        if(input) {
            switch(line) {
            case 'P':
                cout << "Retrieving player..." << endl;
                generation.AddAsPerson(RetrievePlayer(input));
                break;
            case 'T':
                cout << "Retrieving trainer..." << endl;
                generation.AddAsTrainer(RetrievePlayer(input));
                break;
            default:
                cout << "Error in file format. (Unknown Char" << line << " )" << endl;
                return false;
            }
        }
    }

    input.close();

    cout << "Loaded successfully!" << endl;

    return true;
}

bool Persistence::Save(Generation& generation)
{
    cout << "Saving data to " << file << endl;

    ofstream output;
    output.open(file.c_str());
    if(output.fail()) {
        cerr << "Error opening file to write." << endl;
        return false;
    }

    int i;
    Thinker *t;

    i = 0;
    t = generation.GetPlayer(i);
    while(t != nullptr){

        output << 'P' << endl;
        SavePlayer(output, t);
        i++;

        cout << "Player Saved !" << endl;
        t = generation.GetPlayer(i);
    }

    i = 0;
    t = generation.GetTrainer(i);
    while(t != nullptr){

        output << 'T' << endl;
        SavePlayer(output, t);
        i++;

        cout << "Trainer Saved!" << endl;
        t = generation.GetTrainer(i);
    }

    output.close();

    cout << "Saved Successfully!" << endl;

    return true;
}

bool Persistence::SaveGraph(float data[], int length, string fname, string title)
{
    cout << "Saving " << title << " learning curve to " << fname << endl;

    ofstream output;
    output.open(fname.c_str());
    if(output.fail()) {
        cerr << "Error opening file to write." << endl;
        return false;
    }
    for(int i = 0; i < length; i++) {
        output << data[i] << endl;
    }

    output.close();

    cout << "Saved Successfully!" << endl;

    return true;
}

bool Persistence::SaveGraph(int data[], int length, string fname, string title)
{
    cout << "Saving " << title << " learning curve to " << fname << endl;

    ofstream output;
    output.open(fname.c_str());
    if(output.fail()) {
        cerr << "Error opening file to write." << endl;
        return false;
    }
    for(int i = 0; i < length; i++) {
        output << data[i] << endl;
    }

    output.close();

    cout << "Saved Successfully!" << endl;

    return true;
}

bool Persistence::SavePlayer(ofstream& output, Thinker* player)
{
    output << player->WeightsToString();

    return true;
}

Thinker* Persistence::RetrievePlayer(ifstream& input)
{
    if(!input) {
        cerr << "Error opening file to write." << endl;
        throw new runtime_error("Error in file format. (Obtaining Parameters. Fully Empty.)");
    }

    string data;
    uint8_t params[PARAMETERS];
    int byte;

    for(int i = 0; i<PARAMETERS; i++) {

        getline(input, data);
        //input.ignore(100, '\n');

        stringstream convert(data); // stringstream used for the conversion initialized with the contents of data.

        if ( !(convert >> byte) ) {
            cerr << "Error in file format. (Obtaining Parameters. NaN.)" << endl;
            throw new runtime_error("Error in file format. (Obtaining Parameters. NaN.)");
        }

        if(!input) {
            cerr << "Error in file format. (Obtaining Parameters. Empty.)" << endl;
            throw new runtime_error("Error in file format. (Obtaining Parameters. Empty.)");
        }

        params[i] = byte;
    }

    Thinker* t = new Thinker(params);

    return t;
}
