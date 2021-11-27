#include<bits/stdc++.h>
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<thread>
#include<chrono>
#include <ctime>
#include <ratio>
#include "Headers/Shader.h"


using namespace std;
using namespace std::chrono;
using std::this_thread::sleep_for;

const int width =600;
const int height= 600;
bool stampede_detected=false;
const int threshold_value=30;
GLFWwindow *window;
mutex m;


void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);  

//people creation

//we have total three kind of entities:
// entity 1 takes 2 unit of time for 1 step
// entity 2 takes 1.5 unit of time for 1 step
//entity 3 takes 1.0 unit of time for 1 step 
// 1 step is equal to 0.1 unit.


struct people{
    int id;
    float x=-1.0,y=0;
    float sp_x=0,sp_y=-1,sp_yy=1;
    float step;
    high_resolution_clock::time_point times ;
    high_resolution_clock::time_point sp_t ;

    //constructor to initialize entity
    people(int a,float h,high_resolution_clock::time_point curr)
    {
        id=a;
        step =h;
        times=curr;
    }
};


//comparision function for make a comparison in priority queue
struct mycomp{
    bool operator()(people const &p1,people const &p2)
    {
      return  p1.step>p2.step;
    } 
};

//min heap based priority queue of peoples.
priority_queue<people,vector<people>,mycomp>pq;

void people_generation()
{

while(!stampede_detected)
{
    //generation how many peoples will enter
    int n =(rand()%15)+10;
    int count =0;
    // m.lock();
    while(count<n)
    {
        int a = rand()%5+1;
        float LO=-0.45, HI=-0.09;
        //generation of y coordinate for 180 degree road
        float r3 = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
        if(a==0) continue;
        if(a==1)
        {
            // time_t ttime = time(0);
            people p(a,2.0,high_resolution_clock::now());
            p.y=r3;

            pq.push(p);
        }
        else if(a==2)
        {
            people p(a,1.5,high_resolution_clock::now());
            p.y=r3;
            pq.push(p);

        }
        else if(a==4)
        {
         //same thing for 45 degree road   
        int ab = rand()%3+1;
        float low= -0.85,high=-0.55;
        if(ab==1)
        {

            people p(a,2.0,high_resolution_clock::now());
            p.y=-0.8;
            p.x=low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
            cout<<"x= "<<p.x<<"y= "<<p.y<<endl;
            pq.push(p);
        }
        else
        if(ab==2)
        {

            people p(a,1.5,high_resolution_clock::now());
            p.y=-0.8;
            p.x=low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
            cout<<"x= "<<p.x<<"y= "<<p.y<<endl;
           
            pq.push(p);
        }
        
else
{
    

            people p(a,1.0,high_resolution_clock::now());
            p.y=-0.8;
            p.x=low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
            cout<<"x= "<<p.x<<"y= "<<p.y<<endl;
           
            pq.push(p);
        
}

 
        }
 
 else if(a==5)
        {
         //same thing for 30 degree road   

            int ab = rand()%3+1;
        float low= -0.7,high=-0.3;
        if(ab==1)
        {

            people p(a,2.0,high_resolution_clock::now());
            p.y=0.3;
            p.x=low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
            cout<<"x= "<<p.x<<"y= "<<p.y<<endl;
            pq.push(p);
        }
        else
        if(ab==2)
        {

            people p(a,1.5,high_resolution_clock::now());
            p.y=0.3;
            p.x=low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
            cout<<"x= "<<p.x<<"y= "<<p.y<<endl;
           
            pq.push(p);
        }
        else
        {
            people p(a,1.0,high_resolution_clock::now());
            p.y=0.3;
            p.x=low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
            cout<<"x= "<<p.x<<"y= "<<p.y<<endl;
           
            pq.push(p);

        }
        }
 
        else{
            
            people p(3,1.0,high_resolution_clock::now());
            p.y=r3;

            pq.push(p);

        }
        
        int r= rand()%101+100;
         int TIME_TO_SLEEP = r;
                sleep_for(std::chrono::milliseconds(TIME_TO_SLEEP));
        count++;
        
    }
    // m.unlock();
}
}

int display_render()
{
    //ensuring that this function always start a little bit late from people generation.
    this_thread::sleep_for(chrono::milliseconds(120));

    //window initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height,"stampede detection",NULL,NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(glewInit()!=GLEW_OK)
    {
        cout<<"glew initialisation failed"<<endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0,0,width,height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    Shader ourShader("drdo.vs","drdo.fs");
    //major task till shader compilation has been done from the line 197 to 217
   //vector for all the peoples who are to be displayed
    vector<people> vertix_outer;
    vector<int>index_outer;
    int cnt=0;
    cout<<"hey"<<pq.size()<<"printing from display_rendering"<<endl;


  while(!glfwWindowShouldClose(window))
    {
        int arr[5]={0,0,0,0,0};
        this_thread::sleep_for(chrono::milliseconds(100));
        m.lock();
            while ( pq.empty()==false and ((duration_cast<microseconds>(high_resolution_clock::now()-pq.top().times)).count()/1000000>=pq.top().step))
            {
                vertix_outer.push_back(pq.top());
                index_outer.push_back(cnt++);
                pq.pop();
            }
            // int n =index_outer.size();
            
            float vertix[6*cnt];
            for(int i=0,j=0;i<cnt;i++)
            {
                    float dis= int((((duration_cast<microseconds>(high_resolution_clock::now()-vertix_outer[i].times)).count())/1000000)/vertix_outer[i].step)*0.07;

                bool flag=false;
                if(vertix_outer[i].id==4)
                {
                    if(vertix_outer[i].sp_y>-0.5)
                    flag=true;
                    else{
                        vertix[j]=vertix_outer[i].x+ dis/1.414;
                        vertix_outer[i].sp_x=vertix[j];
                        vertix_outer[i].sp_t=high_resolution_clock::now();
                        j++;
                        vertix[j]=vertix_outer[i].y+ dis/1.414;
                        vertix_outer[i].sp_y=vertix[j];
                        j++;
                        vertix[j]=0;
                        j++;
                    }

                }
                else if(vertix_outer[i].id==5)
                {
                    if(vertix_outer[i].sp_yy<0.0)
                    flag=true;
                    else{
                            vertix[j]=vertix_outer[i].x+ (dis*1.732)/2.0;
                            vertix_outer[i].sp_x=vertix[j];
                            vertix_outer[i].sp_t=high_resolution_clock::now();
                            j++;
                            vertix[j]=vertix_outer[i].y- dis/2.0;
                            vertix_outer[i].sp_yy=vertix[j];
                            j++;
                            vertix[j]=0;
                            j++;
                        }

                }
                 if(vertix_outer[i].id==1 or vertix_outer[i].id==2 or vertix_outer[i].id==3  or flag==true)
                {
                    if(vertix_outer[i].id>=4)
                    {
                       vertix[j]=vertix_outer[i].sp_x+int((((duration_cast<microseconds>(high_resolution_clock::now()-vertix_outer[i].sp_t)).count())/1000000)/vertix_outer[i].step)*0.07;
                    }
                    
                    else
                        {
                        vertix[j]=vertix_outer[i].x+int((((duration_cast<microseconds>(high_resolution_clock::now()-vertix_outer[i].times)).count())/1000000)/vertix_outer[i].step)*0.07;
                        }
                if(vertix[j]>=-1.0 and vertix[j]<=-0.6)
                arr[0]++;
                else if(vertix[j]>-0.6 and vertix[j]<=-0.2)
                arr[1]++;
                else if(vertix[j]>-0.2 and vertix[j]<=0.2)
                arr[2]++;
                else if(vertix[j]>=0.2 and vertix[j]<=0.6)
                arr[3]++;
                else if(vertix[j]>0.6 and vertix[j]<=1.0)
                arr[4]++;
                j++;
                if(vertix_outer[i].id<4)
                vertix[j]=vertix_outer[i].y;
                else if(vertix_outer[i].id==4)
                vertix[j]=vertix_outer[i].sp_y;
                else
                vertix[j]=vertix_outer[i].sp_yy;

                j++;
                vertix[j]=0;
                j++;
                }

                // vertix[j++]=vertix_outer[i].id;
                cout<<"I am "<<vertix_outer[i].id<<endl;
                if(vertix_outer[i].id==5 or vertix_outer[i].id==4 )
                {
                    vertix[j++]=1.0f;
                    vertix[j++]=0.0f;
                    vertix[j++]=0.0f;

                }
                else if(vertix_outer[i].id==2 or vertix_outer[i].id==1)
                {
                    vertix[j++]=0.0f;
                    vertix[j++]=1.0f;
                    vertix[j++]=0.0f;
                }
                else
                {
                    vertix[j++]=0.0f;
                    vertix[j++]=0.0f;
                    vertix[j++]=1.0f;

                }

                            }

            // cout<<endl<<"arya123"<<endl;
            //             for(int i=0;i<cnt;i++)
            //             {
            //                     for(int j=0;j<4;j++)
            //                     cout<<vertix[i*4+j]<<" ";
            //                     cout<<endl;
            //             }    
            //         cout<<endl<<"arya124"<<endl;


            unsigned int index[cnt];
            for(int i=0;i<cnt;i++)
            index[i]=i;
            for(int i=0;i<cnt*3;)
            {

                cout<<vertix[i++]<<" "<<vertix[i++]<<" "<<vertix[i++]<<endl;

            }

            
     

    float vertices[]={
        -1.0f,0.0f,0.0f,-0.2f,0.0f,0.0f, -0.7196f,0.3f,0.0f, 0.2f,0.0f,0.0f,  
 -0.3196f,0.3f,0.0f, 1.0f,0.0f,0.0f, -1.0f,-0.5f,0.0f, -0.6f,-0.5f,0.0f,
       -0.9f,-0.8f,0.0f,  -0.2f,-0.5f,0.0f, -0.5f,-0.8f,0.0f, 1.0f,-0.5f,0.0f
    };
    unsigned int indices[] = {  // note that we start from 0!
    0, 1, 1,2,3,4,3,5,6,7,7,8,9,10,9,11};

unsigned int VAO;
glGenVertexArrays(1, &VAO);
glBindVertexArray(VAO);  
unsigned int EBO;
glGenBuffers(1, &EBO);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
    cout<<"printing size of indices "<<sizeof(indices)<<endl;
    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void *)0);
    glEnableVertexAttribArray(0);
        processInput(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ourShader.use();
        glBindVertexArray(VAO);
            glPointSize(9);
            glLineWidth(13);
            glDrawElements(GL_LINES,16, GL_UNSIGNED_INT, 0); 
 
 
   

unsigned int VAO2;
glGenVertexArrays(1, &VAO2);
glBindVertexArray(VAO2);

unsigned int EBO2;
glGenBuffers(1, &EBO2);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index), index, GL_STATIC_DRAW); 


    unsigned int VBO2;
    glGenBuffers(1,&VBO2);
    glBindBuffer(GL_ARRAY_BUFFER,VBO2);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertix),vertix,GL_STATIC_DRAW);


    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void *)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    


 
 
        glBindVertexArray(VAO2);


        glDrawArrays(GL_POINTS,0,cnt);
        // glDrawElements(GL_POINTS,cnt,GL_UNSIGNED_INT,0);


    if(arr[0]>=threshold_value or arr[1]>=threshold_value or arr[2]>=threshold_value or arr[3]>=threshold_value or arr[4]>=threshold_value )
    {
            stampede_detected=true;
            if(arr[0]>=threshold_value)
            {
                cout<<"first region"<<endl;
                cout<<"total "<<arr[0]<<endl;
            }
            else if(arr[1]>=threshold_value)
            {
                cout<<"second region"<<endl;
                cout<<"total "<<arr[1]<<endl;
            }
            else if(arr[2]>=threshold_value)
            {
                cout<<"third region"<<endl;
                cout<<"total "<<arr[2]<<endl;
            }

            else if(arr[3]>=threshold_value)
            {
                cout<<"foutth region"<<endl;
                cout<<"total "<<arr[3]<<endl;
            }

            else
            {
                cout<<"fifth region"<<endl;
                cout<<"total "<<arr[4]<<endl;
            }
           
            this_thread::sleep_for(chrono::milliseconds(120000));
            return 0;

}
    

    glfwSwapBuffers(window);


    glfwPollEvents();    
            m.unlock();
    
    }

    return 0;
}

int main()
{
     srand(time(0));    
     glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    //creation of main window
    window = glfwCreateWindow(width, height,"stampede detection",NULL,NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //glew initialisation
    if(glewInit()!=GLEW_OK)
    {
        cout<<"glew initialisation failed"<<endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0,0,width,height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    thread t2(people_generation);
    thread t1(display_render);
    t2.join();
    t1.join();
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int widt, int heigh)
{
    glViewport(0, 0, widt, heigh);
}  
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}