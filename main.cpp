#include<iostream>
#include<cstdlib>
#include<winsock.h>
#include<mysql.h>
#include<windows.h>
#include<conio.h>
#include<sstream>
#include<bits/stdc++.h>
using namespace std;
void gotoxy (int x, int y)
    {
         COORD coordinates;     // coordinates is declared as COORD
         coordinates.X = x;     // defining x-axis
         coordinates.Y = y;     //defining  y-axis
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coordinates);
    }
struct metro_graph
{
    list<int>graph[53];
};
int homepage()
{
    int choice;
    gotoxy(60,0);
    cout<<"    METRO INFORMATION SYSTEM";
    gotoxy(70,2);
    cout<<"DELHI METRO"<<endl;
    for(int i=0;i<149;i++)
        cout<<"*";
    gotoxy(30,7);
    cout<<"1.FARE INFORMATION";gotoxy(30,9);
    cout<<"2.STATION INFORMATION";gotoxy(30,11);
    cout<<"3.LOST AND FOUND";gotoxy(30,13);
    cout<<"4.SOFTWARE DETAILS";gotoxy(30,15);
    cout<<"5.CREDITS";gotoxy(30,17);
    cout<<"6.EXIT";gotoxy(30,19);
    gotoxy(80,18);
    cout<<"Please enter your valid choice: ";
    cin>>choice;
    return choice;
}
metro_graph* create_graph()
{
    struct metro_graph*adj_list=new metro_graph;
    for(int i=0;i<=52;i++)
    {
        if(i>1)
        adj_list->graph[i].push_back(i-1);
        if(i!=52)
        adj_list->graph[i].push_back(i+1);
    }
    adj_list->graph[21].push_back(46);
    adj_list->graph[46].push_back(21);
    adj_list->graph[32].push_back(42);
    adj_list->graph[42].push_back(32);
    return adj_list;
}
void print_list(metro_graph*adj_list)
{
    for(int i=0;i<53;i++)
    {
        cout<<i<<"->";
        if(!adj_list->graph[i].empty())
        {
            list<int>::iterator it;
            for(it=adj_list->graph[i].begin();it!=adj_list->graph[i].end();it++)
                cout<<*it<<"->";
        }
        cout<<endl;
    }
}
int calculate_fare(int bid,int did,metro_graph*adj_list)
{
    int nos=0;
    if((bid>=1&&did<=26)||(bid>=27&&did<=37)||(bid>=38&&did<=52))
        return bid>did?(bid-did):(did-bid);
    list<int>q;
    bool done=false;
            bool visited[53];
            int level[53];
            int l=1;
            int temp=0;
            fill(level,level+53,0);
            fill(visited,visited+53,false);
            visited[bid]=true;
            q.push_back(bid);
            list<int>::iterator it;
            while(!q.empty())
            {
                bid=q.front();
                q.pop_front();
                for(it=adj_list->graph[bid].begin();it!=adj_list->graph[bid].end();it++)
                {
                    if(!visited[*it])
                    {
                        visited[*it]=true;
                        q.push_back(*it);
                        level[*it]=l;
                    }
                }
                temp++;
                if(temp%2==0)
                    l++;

                if(bid==did)
                    break;
            }
        nos=(level[did]/2+1)*5;
        return nos;
}
void fare_info()
{
    int choice;
    gotoxy(65,0);
    cout<<"    METRO INFORMATION SYSTEM";
    gotoxy(75,2);
    cout<<"DELHI METRO"<<endl;
    for(int i=0;i<168;i++)
        cout<<"*";
    MYSQL *conn;
    MYSQL_ROW row1;
    MYSQL_ROW row2;
    MYSQL_RES *res;
    MYSQL_RES *res2;
    conn=mysql_init(0);
    conn=mysql_real_connect(conn,"localhost","root","","metro",0,NULL,0 );
    if(conn)
    {
        gotoxy(75,5);
        cout<<"FARE INFORMATION "<<endl;
        gotoxy(25,7);
        cout<<"Enter boarding station name : ";
        gotoxy(105,7);
        cout<<"Enter destination station name : ";
        cin.ignore(numeric_limits<streamsize>::max(),'\n'); //to ignore \n as input buffer
        string b_name,d_name;
        gotoxy(55,7);
        getline(cin,b_name);
        gotoxy(137,7);
        getline(cin,d_name);
        string q1="Select id from blue_line where sname like '%"+b_name+"%'";
        const char *query1=q1.c_str();
        int bid=0;
        int did=0;
        int i=0;
        int qstate1=mysql_query(conn,query1);
        if(qstate1==0)
            {
                res=mysql_store_result(conn);
                while(row1=mysql_fetch_row(res))
                    {
                          bid=atoi(row1[0]);
                          if(bid)
                            break;
                    }
            }
            if(bid==0)
               {
                   gotoxy(70,11);
                   cout<<"INVALID BOARDING STATION NAME!! ";
                }
        string q2="Select id from blue_line where sname like '%"+d_name+"%'";
        const char *query2=q2.c_str();
        int qstate2=mysql_query(conn,query2);
        if(qstate2==0)
        {
            res2=mysql_store_result(conn);
            while(row2=mysql_fetch_row(res2))
            {
                did=atoi(row2[0]);
                if(did)
                    break;
            }
        }
        if(did==0)
        {
            gotoxy(70,11);
            cout<<"INVALID DESTINATION STATION NAME!!";
        }
        if(bid!=0&&did!=0)
        {
            metro_graph*graph=create_graph();
            int fare=calculate_fare(bid,did,graph);
            transform(b_name.begin(), b_name.end(), b_name.begin(), ::toupper);
            transform(d_name.begin(), d_name.end(), d_name.begin(), ::toupper);
            gotoxy(38,18);
            for(int i=0;i<70;i++)
                cout<<"_";
            for(int i=19;i<24;i++)
            {
                gotoxy(37,i);
                cout<<"|";
            }
            gotoxy(38,23);
            for(int i=0;i<70;i++)
                cout<<"_";
            for(int i=19;i<24;i++)
            {
                gotoxy(108,i);
                cout<<"|";
            }
            gotoxy(40,20);
            cout<<"FROM STATION ";
            gotoxy(39,22);
            cout<<b_name;
            gotoxy(60,20);
            cout<<"TO STATION ";
            gotoxy(59,22);
            cout<<d_name;
            gotoxy(80,20);
            cout<<"FARE ";
            gotoxy(81,22);
            cout<<fare;
            gotoxy(90,20);
            cout<<"CHANGING STATION";
            gotoxy(89,22);
            cout<<"RAJEEV CHOWK";
            cout<<"\n\n\n";
            cout<<"Press 1 to return and 0 to exit:";
            cin>>choice;
            if(choice==1)
            {

            }
            else
                return ;
        }

    }
    else
        cout<<"SERVER NOT CONNECTED!!";
}
void station_info()
{
    gotoxy(65,0);
    cout<<"    METRO INFORMATION SYSTEM";
    gotoxy(75,2);
    cout<<"DELHI METRO"<<endl;
    for(int i=0;i<168;i++)
        cout<<"*";
    MYSQL *conn;
    MYSQL_ROW row1;
    MYSQL_ROW row2;
    MYSQL_RES *res;
    MYSQL_RES *res2;
    conn=mysql_init(0);
    conn=mysql_real_connect(conn,"localhost","root","","metro",0,NULL,0 );
    if(conn)
    {
        string name;
        int i=1,j=8;
        gotoxy(75,5);
        cout<<"STATION INFORMATION "<<endl;
        string q1="Select sname from blue_line order by sname";
        const char *query1=q1.c_str();
        int qstate1=mysql_query(conn,query1);
        if(qstate1==0)
            {
                res=mysql_store_result(conn);
                while(row1=mysql_fetch_row(res))
                    {
                        if(i<=13)
                        {
                            gotoxy(2,j);
                            cout<<row1[0];
                            i++;
                            j+=2;
                        }
                        else if(i>13&&i<=26)
                        {
                            if(i==14){j=8;}
                            gotoxy(40,j);
                            cout<<row1[0];
                            i++;
                            j+=2;
                        }
                        else if(i>26&&i<=39)
                        {
                            if(i==27){j=8;}
                            gotoxy(80,j);
                            cout<<row1[0];
                            i++;
                            j+=2;
                        }
                        else if(i>39&&i<=52)
                        {
                            if(i==40){j=8;}
                            gotoxy(120,j);
                            cout<<row1[0];
                            i++;
                            j+=2;
                        }
                    }
            }
            gotoxy(30,40);
            cout<<"Enter station name to view details: ";
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            getline(cin,name);
            system("CLS");
            gotoxy(65,0);
            cout<<"    METRO INFORMATION SYSTEM";
            gotoxy(75,2);
            cout<<"DELHI METRO"<<endl;
            for(int i=0;i<168;i++)
            cout<<"*";
            q1="Select * from blue_line where sname like '%"+name+"%'";
            query1=q1.c_str();
            int sid=0;
            qstate1=mysql_query(conn,query1);
            if(conn)
            {
                string line,route,junction="NULL";
                res=mysql_store_result(conn);
                while(row1=mysql_fetch_row(res))
                {
                    sid=atoi(row1[0]);
                    gotoxy(70,4);
                    cout<<row1[1];
                    if(sid!=0)
                        break;
                }
                if(sid>=1&&sid<=26){line="Blue"; route="Noida Electronic city <-> Dwarka Sector 21";}
                if(sid>=27&&sid<=37){line="Red"; route="Shaheed Sthal <-> Rithala";}
                if(sid>=38&&sid<=52){line="Yellow"; route="Samaypur Badli <-> Huda City Centre";}
                if(sid==21||sid==46){junction="Yellow line and Blue line";}
                if(sid==32||sid==42){junction="Yellow line and Red line";}
                gotoxy(30,7);
                cout<<"LINE :"<<line;gotoxy(30,9);
                cout<<"ROUTE :"<<route;gotoxy(30,11);
                cout<<"TIMING :6:00AM-11::00PM";gotoxy(30,13);
                cout<<"CONTACT NUMBER :+919999999999";gotoxy(30,15);
                cout<<"JUNCTION FOR :"<<junction;gotoxy(30,17);
            }

    }
}
void lost_found()
{
    int choice;
    gotoxy(65,0);
    cout<<"    METRO INFORMATION SYSTEM";
    gotoxy(75,2);
    cout<<"DELHI METRO"<<endl;
    for(int i=0;i<168;i++)
        cout<<"*";
    MYSQL *conn;
    MYSQL_ROW row1;
    MYSQL_ROW row2;
    MYSQL_RES *res;
    MYSQL_RES *res2;
    conn=mysql_init(0);
    conn=mysql_real_connect(conn,"localhost","root","","metro",0,NULL,0 );
    if(conn)
    {
        gotoxy(70,5);
        cout<<"LOST AND FOUND ONLINE-PORTAL"<<endl;
        gotoxy(44,9);
        cout<<"Press 0 if you have lost your belongings or Press 1 if you found !!";
        cin>>choice;
        if(choice==0)
        {
            int temp=0;
            string name,color,station,company;
            system("CLS");
            gotoxy(65,0);
            cout<<"    METRO INFORMATION SYSTEM";
            gotoxy(75,2);
            cout<<"DELHI METRO"<<endl;
            for(int i=0;i<168;i++)
            cout<<"*";
            gotoxy(70,5);
            cout<<"LOST AND FOUND ONLINE-PORTAL"<<endl;gotoxy(44,9);
            cout<<"Please provide us few information of article(s) lost to search for it in our database!";
            gotoxy(30,11);
                cout<<"Item Name: ";
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                getline(cin,name);gotoxy(30,13);
                cout<<"Item Color: ";
                getline(cin,color);gotoxy(30,15);
                cout<<"Lost Station: ";
                getline(cin,station);gotoxy(30,17);
                cout<<"Company: ";
                getline(cin,company);gotoxy(30,19);
                gotoxy(40,12);
                string q1="Select name from lost_found where name='"+name+"' and station ='"+station+"'";
                const char *query1=q1.c_str();
                int qstate1=mysql_query(conn,query1);
                if(qstate1==0)
                {
                    res=mysql_store_result(conn);
                    while(row1=mysql_fetch_row(res))
                    {
                        temp++;
                    }
                    gotoxy(50,23);
                    if(temp!=0)
                    {
                        cout<<"Such "<<temp<<" items are found. A separate lost and found\n";gotoxy(50,25);
                        cout<<"office is set up at Kashmere gate Metro Station.";gotoxy(50,27);
                        cout<<"The lost item will be handed over to claimant only";gotoxy(62,29);
                        cout<<" after proper verification ";
                    }
                    if(temp==0){
                            gotoxy(55,23);
                        cout<<"No such item is found in our record";
                    }

                }
        }
        if(choice==1)
        {
            int temp=0;
            string name,color,station,company;
            system("CLS");
            gotoxy(65,0);
            cout<<"    METRO INFORMATION SYSTEM";
            gotoxy(75,2);
            cout<<"DELHI METRO"<<endl;
            for(int i=0;i<168;i++)
            cout<<"*";
            gotoxy(70,5);
            cout<<"LOST AND FOUND ONLINE-PORTAL"<<endl;gotoxy(46,9);
            cout<<"Please provide us few information of article(s) found ";
            gotoxy(30,11);
                cout<<"Item Name: ";
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                getline(cin,name);gotoxy(30,13);
                cout<<"Station: ";
                getline(cin,station);gotoxy(30,17);
                gotoxy(40,12);
                string q1="insert into lost_found values('"+name+"','"+station+"')";
                const char *query1=q1.c_str();
                int qstate1=mysql_query(conn,query1);
                if(qstate1==0)
                {
                    gotoxy(50,15);
                    cout<<"Thanks for making delhi metro more trustful for user!";
                }
        }
    }
}
void software_info()
{
    gotoxy(65,0);
    cout<<"    METRO INFORMATION SYSTEM";
    gotoxy(75,2);
    cout<<"DELHI METRO"<<endl;
    for(int i=0;i<168;i++)
        cout<<"*";
    gotoxy(70,5);
    cout<<"SOFTWARE INFORMATION"<<endl;
    gotoxy(70,8);
    cout<<"------SERVICES------ ";
    int j=12;
    gotoxy(70,46);
    cout<<"------METHODOLOGY------";
    fstream newfile;
    newfile.open("software_infot.txt",ios::in);
    if (newfile.is_open())
    {
      string tp;
      gotoxy(50,j);
      while(getline(newfile, tp)){
         gotoxy(50,j);
         cout<<tp;
         j+=2;
      }
      newfile.close(); //close the file object.
   }
}
void credits()
{
    gotoxy(65,0);
    cout<<"    METRO INFORMATION SYSTEM";
    gotoxy(75,2);
    cout<<"DELHI METRO"<<endl;
    for(int i=0;i<149;i++)
        cout<<"*";
    gotoxy(78,5);
    cout<<"CREDITS"<<endl;
    gotoxy(70,8);
    fstream newfile;
    newfile.open("credits.txt",ios::in);
    int j=8;
    if (newfile.is_open())
    {
      string tp;
      gotoxy(50,j);
      while(getline(newfile, tp)){
         gotoxy(50,j);
         cout<<tp;
         j+=2;
      }
    }
      newfile.close();
}
int main()
{
    homepage:int choice=homepage();
    if(choice<1||choice>6)
    {
        system("CLS");
        goto homepage;
    }
    switch(choice)
    {
    case 1:
        system("CLS");
        fare_info();
        _getch();
        system("CLS");
        goto homepage;
        break;
    case 2:
        system("CLS");
        station_info();
        _getch();
        system("CLS");
        goto homepage;
        break;
    case 3:
        system("CLS");
        lost_found();
        _getch();
        system("CLS");
        goto homepage;
        break;
    case 4:
        system("CLS");
        software_info();
        _getch();
        system("CLS");
        goto homepage;
        break;
    case 5:
        system("CLS");
        credits();
        _getch();
        system("CLS");
        goto homepage;
        break;
    default:
        exit(0);
    }
}
