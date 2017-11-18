#include "MeshReaderUnv.h"


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

        case 91:
                type_faces.push_back(91);
                sscanf(it->c_str(),"%d %d %d", &tmp[0], &tmp[1], &tmp[2]);
                ++it;

                p.push_back(--tmp[0]);
                p.push_back(--tmp[1]);
                p.push_back(--tmp[2]);

                faces.push_back(p);
                break;

        case 94:
                type_faces.push_back(94);
                sscanf(it->c_str(),"%d %d %d %d", &tmp[0], &tmp[1], &tmp[2], &tmp[3]);
                ++it;

                p.push_back(--tmp[0]);
                p.push_back(--tmp[1]);
                p.push_back(--tmp[2]);
                p.push_back(--tmp[3]);

                faces.push_back(p);
                break;

        case 111:
                type_cells.push_back(111);
                sscanf(it->c_str(),"%d %d %d %d", &tmp[0],&tmp[1],&tmp[2],&tmp[3]);
                ++it;

                p.push_back(--tmp[0]);
                p.push_back(--tmp[1]);
                p.push_back(--tmp[2]);
                p.push_back(--tmp[3]);

                cells.push_back(p);
                break;

        case 115:
                type_cells.push_back(115);
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


void MeshReaderUnv::read(Mesh** mesh)
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

    (*mesh) = new Mesh(points);

    for(vector<vector<int> >::iterator it = edges.begin(); it != edges.end(); ++it) {

        (*mesh)->createEdge((*it)[0], (*it)[1]);
    }

    vector<int>::iterator typeitf = type_faces.begin();
    for(vector<vector<int> >::iterator it = faces.begin(); it != faces.end(); ++it, ++typeitf) {

        switch(*typeitf) {
            case 91:
             (*mesh)->createFace(*typeitf, (*it)[0], (*it)[1], (*it)[2]);
             break;
            case 94:
             (*mesh)->createFace(*typeitf, (*it)[0], (*it)[1], (*it)[2],(*it)[3]);
             break;
        }
    }


    int k;
    vector<int> indFace1;
    vector<int> indFace2;

    vector<int>::iterator typeitc = type_cells.begin();
    for(vector<vector<int> >::iterator it = cells.begin(); it != cells.end(); ++it, ++typeitc) {

        indFace1.clear();
        indFace2.clear();

     switch(*typeitc) {

         case 111:
            {

             for(k = 0; k < 3; k++) {
                indFace1.push_back((*it)[k]);
                indFace2.push_back((*it)[k + 1]);
             }

             if(!face_is_exist(set<int>(indFace1.begin(), indFace1.end()))) {
                faces.push_back(vector<int>(indFace1.begin(), indFace1.end()));
                (*mesh)->createFace(0, indFace1[0], indFace1[1], indFace1[2]);
             }

             if(!face_is_exist(set<int>(indFace2.begin(), indFace2.end()))) {
                faces.push_back(vector<int>(indFace2.begin(), indFace2.end()));
                (*mesh)->createFace(0, indFace2[0], indFace2[1], indFace2[2]);
             }

             indFace1[2] = (*it)[3];
             indFace2[0] = (*it)[0];


             if(!face_is_exist(set<int>(indFace1.begin(), indFace1.end()))) {
                faces.push_back(vector<int>(indFace1.begin(), indFace1.end()));
                (*mesh)->createFace(0, indFace1[0], indFace1[1], indFace1[2]);
             }

             if(!face_is_exist(set<int>(indFace2.begin(), indFace2.end()))) {
                faces.push_back(vector<int>(indFace2.begin(), indFace2.end()));
                (*mesh)->createFace(0, indFace2[0], indFace2[1], indFace2[2]);
             }

             vector<int> temp_edge;

             temp_edge.clear();
             if(!edge_is_exist((*it)[0], (*it)[1])) {

                temp_edge.push_back((*it)[0]);
                temp_edge.push_back((*it)[1]);
                edges.push_back(temp_edge);
                (*mesh)->createEdge((*it)[0], (*it)[1]);
             }

             temp_edge.clear();
             if(!edge_is_exist((*it)[1], (*it)[2])) {

                temp_edge.push_back((*it)[1]);
                temp_edge.push_back((*it)[2]);
                edges.push_back(temp_edge);
                (*mesh)->createEdge((*it)[1], (*it)[2]);
             }

             temp_edge.clear();
             if(!edge_is_exist((*it)[0], (*it)[2])) {

                temp_edge.push_back((*it)[0]);
                temp_edge.push_back((*it)[2]);
                edges.push_back(temp_edge);
                (*mesh)->createEdge((*it)[0], (*it)[2]);
             }

             temp_edge.clear();
             if(!edge_is_exist((*it)[0], (*it)[3])) {

                temp_edge.push_back((*it)[0]);
                temp_edge.push_back((*it)[3]);
                edges.push_back(temp_edge);
                (*mesh)->createEdge((*it)[0], (*it)[3]);
             }

             temp_edge.clear();
             if(!edge_is_exist((*it)[1], (*it)[3])) {

                temp_edge.push_back((*it)[1]);
                temp_edge.push_back((*it)[3]);
                edges.push_back(temp_edge);
                (*mesh)->createEdge((*it)[1], (*it)[3]);
             }

             temp_edge.clear();
             if(!edge_is_exist((*it)[2], (*it)[3])) {

                temp_edge.push_back((*it)[2]);
                temp_edge.push_back((*it)[3]);
                edges.push_back(temp_edge);
                (*mesh)->createEdge((*it)[2], (*it)[3]);
             }

             (*mesh)->createCell(*typeitc, (*it)[0], (*it)[1], (*it)[2], (*it)[3]);

             break;
         }
         case 115:
         {
             for(k = 0; k < 4; k++) {
                indFace1.push_back((*it)[k]);
                indFace2.push_back((*it)[k + 4]);
            }


            if( !face_is_exist(set<int>(indFace1.begin(), indFace1.end())) ) {

                faces.push_back(vector<int>(indFace1.begin(), indFace1.end()));
                (*mesh)->createFace(0, indFace1[0], indFace1[1], indFace1[2], indFace1[3]);
            }


             if( !face_is_exist(set<int>(indFace2.begin(), indFace2.end())) ) {

                faces.push_back(vector<int>(indFace2.begin(), indFace2.end()));
                (*mesh)->createFace(0, indFace2[0], indFace2[1], indFace2[2], indFace2[3]);
            }

            set<int> Face;

            Face.insert(indFace1[0]); Face.insert(indFace1[1]);
            Face.insert(indFace2[0]); Face.insert(indFace2[1]);

            if( !face_is_exist(Face) ) {

                faces.push_back(vector<int>(Face.begin(), Face.end()));
                (*mesh)->createFace(0 , indFace1[0], indFace1[1], indFace2[0], indFace2[1]);
            }

            Face.clear();

            Face.insert(indFace1[2]); Face.insert(indFace1[3]);
            Face.insert(indFace2[2]); Face.insert(indFace2[3]);

            if( !face_is_exist(Face) ) {

                faces.push_back(vector<int>(Face.begin(), Face.end()));
                (*mesh)->createFace(0, indFace1[2], indFace1[3], indFace2[2], indFace2[3]);
            }

            Face.clear();

            Face.insert(indFace1[1]); Face.insert(indFace1[2]);
            Face.insert(indFace2[1]); Face.insert(indFace2[2]);

            if( !face_is_exist(Face) ) {

                faces.push_back(vector<int>(Face.begin(), Face.end()));
                (*mesh)->createFace(0, indFace1[1], indFace1[2], indFace2[1], indFace2[2]);
            }

            Face.clear();

            Face.insert(indFace1[0]); Face.insert(indFace1[3]);
            Face.insert(indFace2[0]); Face.insert(indFace2[3]);

            if( !face_is_exist(Face) ) {

                faces.push_back(vector<int>(Face.begin(), Face.end()));
                (*mesh)->createFace(0, indFace1[0], indFace1[3], indFace2[0], indFace2[3]);
            }


            vector<int> temp;

            for(k = 0; k < 4; k++) {
                temp.clear();

                if( !edge_is_exist(indFace1[k],indFace1[(k+1)%4]) ) {

                    temp.push_back(indFace1[k]);
                    temp.push_back(indFace1[ (k+1)%4 ]);
                    edges.push_back(temp);
                    (*mesh)->createEdge(temp[0],temp[1]);
                }

                temp.clear();
                if( !edge_is_exist(indFace2[k],indFace2[(k+1)%4]) ) {

                    temp.push_back(indFace2[k]);
                    temp.push_back(indFace2[ (k+1)%4 ]);
                    edges.push_back(temp);
                    (*mesh)->createEdge(temp[0],temp[1]);
                }

                temp.clear();
                if( !edge_is_exist(indFace1[k],indFace2[k]) ) {

                    temp.push_back(indFace1[k]);
                    temp.push_back(indFace2[k]);
                    edges.push_back(temp);
                    (*mesh)->createEdge(temp[0],temp[1]);
                }
            }

            (*mesh)->createCell(*typeitc, indFace1[0], indFace1[1], indFace1[2], indFace1[3], indFace2[0], indFace2[1], indFace2[2], indFace2[3]);
            break;
            }
        }
    }
}
