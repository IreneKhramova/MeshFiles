#include "../include/MeshReaderUnv.h"


void MeshReaderUnv::read_block(vector<string> *listOfstrings, ifstream& fin)
{
    listOfstrings->clear();
    int minusCnt = 0;
    string str;
    int pos;

    while( minusCnt < 2 && !fin.eof())
    {
        getline(fin, str);
        pos = str.find("-1");

         if(pos != -1) {
            minusCnt++;
            continue;
         }

        listOfstrings->push_back(str);
    }

}

void MeshReaderUnv::parse_block_164(vector<string> listOfstrings)
{

}

void MeshReaderUnv::parse_block_2420(vector<string> listOfstrings)
{

}

void MeshReaderUnv::parse_block_2411(vector<string> &listOfstrings)
{
    points.clear();
    vector<string>::iterator it = listOfstrings.begin();
    ++it;

    unsigned int i = 0;
    while(it != listOfstrings.end() )
    {
          if(i % 2 == 1) {
            Point p;

            int pos;
            while( (pos = it->find('D')) != -1) {
                (*it)[pos] = 'E';
            }
            sscanf(it->c_str(), "%lf %lf %lf", &p.x, &p.y, &p.z);

            points.push_back(p);
        }
         ++i;
         ++it;
    }
}

void MeshReaderUnv::parse_block_2412(vector<string> &listOfstrings)
{
    vector<string>::iterator it = listOfstrings.begin();
    ++it;

    int tmp[8];
    vector<int> p;
    while(it != listOfstrings.end())
    {
        p.clear();
        sscanf(it->c_str(),"%d %d %d %d %d %d", &tmp[0], &tmp[1], &tmp[2], &tmp[3], &tmp[4], &tmp[5]);
        ++it;

        switch(tmp[1])
        {
        case 21:
                sscanf(it->c_str(),"%d %d %d", &tmp[0], &tmp[1], &tmp[2]);
                ++it;
                sscanf(it->c_str(),"%d %d", &tmp[0], &tmp[1]);
                ++it;

                p.push_back(--tmp[0]);
                p.push_back(--tmp[1]);

                edges.push_back(p);
                break;

        case 94:
                sscanf(it->c_str(),"%d %d %d %d", &tmp[0], &tmp[1], &tmp[2], &tmp[3]);
                ++it;

                p.push_back(--tmp[0]);
                p.push_back(--tmp[1]);
                p.push_back(--tmp[2]);
                p.push_back(--tmp[3]);

                faces.push_back(p);
                break;

        case 115:
                sscanf(it->c_str(),"%d %d %d %d %d %d %d %d", &tmp[0],&tmp[1],&tmp[2],&tmp[3],&tmp[4],&tmp[5],&tmp[6],&tmp[7]);
                ++it;

                p.push_back(--tmp[0]);
                p.push_back(--tmp[1]);
                p.push_back(--tmp[2]);
                p.push_back(--tmp[3]);
                p.push_back(--tmp[4]);
                p.push_back(--tmp[5]);
                p.push_back(--tmp[6]);
                p.push_back(--tmp[7]);

                cells.push_back(p);
                break;
        }
    }
}

void MeshReaderUnv::parse_block(vector<string> &listOfstrings)
{
    int type = atoi( listOfstrings[0].c_str() );

    switch(type)
    {
        case 164:
            parse_block_164(listOfstrings);
            break;
        case 2420:
            parse_block_2420(listOfstrings);
            break;
        case 2411:
            parse_block_2411(listOfstrings);
            break;
        case 2412:
            parse_block_2412(listOfstrings);
            break;
    }
}


bool MeshReaderUnv::face_is_exist(set<int> face){

    for(vector<vector<int> >::iterator it = faces.begin(); it != faces.end(); ++it) {
        if(face == set<int>((*it).begin(), (*it).end()))
            return true;
    }
    return false;
}

bool MeshReaderUnv::edge_is_exist(int i, int j){

    for(vector<vector<int> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        if( (i == (*it)[0] && j == (*it)[1]) || (i == (*it)[1] && j == (*it)[0]) )
            return true;
    }
    return false;
}

void MeshReaderUnv::createFirstFace(int* indexesOfPoints, vector<int> indFace) {

        int i, j, k;
        int ind1 = -1;
        int ind2;

        i = indFace[0];
        for(k = 1; k < 4; k++) {
            if( !edge_is_exist(i, indFace[k]) ) {
                j = k;
            }
            else {
                if(ind1 == -1)
                    ind1 = k;
                else
                    ind2 = k;
            }
        }

        indexesOfPoints[0] = i;
        indexesOfPoints[1] = indFace[ind1];
        indexesOfPoints[2] = indFace[j];
        indexesOfPoints[3] = indFace[ind2];
}

void MeshReaderUnv::read(Mesh* mesh)
{
    ifstream fin(fileName.c_str());

    if( fin.is_open() ) {

        vector<string> listOfStrings;

        while( !fin.eof() ) {

            read_block(&listOfStrings, fin);
            parse_block(listOfStrings);

        }
        fin.close();
    }
    else {
        ///Error
    }

    Point* temp_masOfpoints = new Point[points.size()];
    for(int i = 0; i < points.size(); i++) {

            temp_masOfpoints[i] = points[i];
    }

    mesh = new Mesh(temp_masOfpoints, points.size()); // создает динамический массив точек размера points.size()

    delete [] temp_masOfpoints;

    for(vector<vector<int> >::iterator it = edges.begin(); it != edges.end(); ++it) {

        mesh->createEdge((*it)[0], (*it)[1]);
    }

    for(vector<vector<int> >::iterator it = faces.begin(); it != faces.end(); ++it) {

        mesh->createFace((*it)[0], (*it)[1], (*it)[2], (*it)[3]);
    }

    int pIndexes[8];// координаты точек для создания cell(нужен определенный порядок)
    int k;
    bool temp_vector[8];// нужен для различия двух противоположных граней
    vector<int> indFace1;
    vector<int> indFace2;

    for(vector<vector<int> >::iterator it = cells.begin(); it != cells.end(); ++it) {

        indFace1.clear();
        indFace2.clear();

        for(k = 0 ; k < 8; k++)
          temp_vector[k] = false;

        for(k = 0; k < 4; k++)
          indFace1.push_back((*it)[k]);

        int vectIndexes[4];
        for(k = 0; k < 4; k++)
          vectIndexes[k] = k + 1;

        int i = 3;
        int j;

        while( !face_is_exist(set<int>(indFace1.begin(), indFace1.end())) && i != -1) { // Генерация сочетания без повторений для нахождения грани

            if(vectIndexes[i] < 5 + i)
            {
               vectIndexes[i]++;
               for(j = i+1; j < 4; j++)
                  vectIndexes[j] = vectIndexes[i] + j - i;

               i = k - 1;
            }
            else
                i--;

            indFace1.clear();
            for(k = 0; k < 8; k++)
                temp_vector[k] = false;

            for(k = 0; k < 4; k++) {
                indFace1.push_back( (*it)[ vectIndexes[k] - 1] );
                temp_vector[ vectIndexes[k] - 1 ] = true;
            }
        }

        if(i == -1)
        {
         ///Error (не получается грань)
        }

        for(k = 0; k < 8; k++)
            if( !temp_vector[k] )
                indFace2.push_back((*it)[k]);

        createFirstFace(pIndexes, indFace1);

        for(k = 0; k < 4; k++) {
            for(vector<int>::iterator fit2 = indFace2.begin(); fit2 != indFace2.end(); ++fit2) {
                if( edge_is_exist(pIndexes[k], *fit2) ) {
                    pIndexes[k + 4] = *fit2;
                    break;
                }
            }
        }

        mesh->createCell(pIndexes[0], pIndexes[1], pIndexes[2], pIndexes[3], pIndexes[4], pIndexes[5], pIndexes[6], pIndexes[7]);
    }
}



