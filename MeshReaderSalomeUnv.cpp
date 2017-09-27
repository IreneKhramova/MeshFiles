#include "MeshReaderSalomeUnv.h"

void MeshReaderSalomeUnv::read_block(vector<string> *listOfstrings, ifstream& fin)
{
    listOfstrings->clear();
    int minusCnt = 0;
    string str;
    int pos;

    while( minusCnt < 2 && !fin.eof())
    {
        getline(fin,str);
        pos = str.find("-1");
         if( pos == str.length() - 2)
         {
            minusCnt++;
            continue;
         }

      listOfstrings->push_back(str);
    }
}

void MeshReaderSalomeUnv::parse_block_164(vector<string> listOfstrings)
{

}

void MeshReaderSalomeUnv::parse_block_2420(vector<string> listOfstrings)
{

}

void MeshReaderSalomeUnv::parse_block_2411(vector<string> listOfstrings)
{
    points.clear();
    vector<string>::iterator it = listOfstrings.begin();
    ++it;

    unsigned int i = 0;
    while(it != listOfstrings.end() && ( (i++) % 2 == 1 ) )
    {
        Point p;
        sscanf(it->c_str(), "%lf %lf %lf", &p.x, &p.y, &p.z);
        points.push_back(p);
        ++it;
    }
}

void MeshReaderSalomeUnv::parse_block_2412(vector<string> listOfstrings)
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
        case 11:
                sscanf(it->c_str(),"%d %d %d", &tmp[0], &tmp[1], &tmp[2]);
                ++it;
                sscanf(it->c_str(),"%d %d", &tmp[0], &tmp[1]);
                ++it;

                p.push_back(--tmp[0]);
                p.push_back(--tmp[1]);

                edges.push_back(p);
                break;

        case 44:
                sscanf(it->c_str(),"%d %d %d %d", &tmp[0], &tmp[1], &tmp[2], &tmp[3]);
                ++it;

                p.push_back(--tmp[0]);
                p.push_back(--tmp[1]);
                p.push_back(--tmp[2]);
                p.push_back(--tmp[3]);

                faces.push_back(p);
                break;

        case 111:
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

void MeshReaderSalomeUnv::parse_block(vector<string> listOfstrings)
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


bool MeshReaderSalomeUnv::face_is_exist(set<int> face){

    for(vector<vector<int> >::iterator it = faces.begin(); it != faces.end(); ++it) {
        if(face == set<int>((*it).begin(), (*it).end()))
            return true;
    }
    return false;
}

bool MeshReaderSalomeUnv::edge_is_exist(int i, int j){

    for(vector<vector<int> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        if( (i == (*it)[0] && j == (*it)[1]) || (i == (*it)[1] && j == (*it)[0]) )
            return true;
    }
    return false;
}

void MeshReaderSalomeUnv::createFace(int* indexesOfPoints, vector<int> indFace) {

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

void MeshReaderSalomeUnv::read(Mesh* mesh)
{
    ifstream fin(fileName);

    if( fin.is_open() ) {

        vector<string> listOfStrings;

        while( !fin.eof() ) {

            read_block(&listOfStrings, fin);
            parse_block(sl);

        }
        fin.close();
    }
    else {
        ///Error
    }

    mesh->createPoints(points.size()); // создает динамический массив точек размера points.size()


    for(vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
        mesh->setPoint(it->x, it->y, it->z); // инициализирует массив точек (нумерация сохраняется !!!)
    }

    mesh->createCells(cells.size()); // создает динамический массив шестигранников размера cell.size()

    int int_tmp[8];// координаты точек для создания cell(нужен определенный порядок)
    int temp, k;
    bool temp_vector[8];// нужен для различия двух противоположных граней
    vector<int> indFace1;
    vector<int> indFace2;

    for(vector<vector<int> >::iterator it = cells.begin(); it != cells.end(); ++it) {

        indFace1.clear();

        for(k = 0 ; k < 8; k++)
          temp_vector[i] = false;

        do {
        for(k = 0; k < 4; k++)
            indFace1.insert(it[k]);


        }while(!face_is_exist(set(indFace1.begin(), indFace1.end())));

        for(k = 0; k < 8; k++)
            if(temp_vector[k])
                indFace2.push_back(it[k]);

        createFace(int_tmp, indFace1);

        for(k = 0; k < 4; k++) {
            temp = int_tmp[k];
            for(vector<int>::iterator fit2 = indFace2.begin(); fit2 != indFace2.end(); ++fit2) {
                if( edge_is_exist(int_tmp[k], *fit2) )
                    int_tmp[k + 4] = *fit2;
            }
        }

        mesh->initCell(tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6], tmp[7]);
    }
}
