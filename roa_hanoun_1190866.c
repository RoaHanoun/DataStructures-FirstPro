/* Department of Computer Science
           COMP2421
           Project #1
  ******** ROA HANOUN || 1190886 ********
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>

typedef struct course*ptr;// pointer of next node in struct course
typedef struct student*poi; // pointer of next node in struct student
// declaration of functions
void readFromFile();
void insert_coruse(ptr);
void insert_student(poi);
ptr findCourse(char *);
poi findStudent(int);
int findIndexOFcourse(char *);
int ifStudentExit(int,int);
int checkNumOFcourcses(poi);
int courseISfull(ptr);
int IFpossible(ptr,poi);
void registerStudent(int,int);
int getIndexWating(char *);
int stuISwating(int,int);
void valid_Remove(int, char *);
void remove_stuFromCor(int, int);
void removeFromWatingList(int,int);
int validationWatingList(int,int);
void validation(int,char *);
void ADD_waitingList(int,int);
void bubbleSort(int);
void Sort_courses();
void print_cource();
void storeToFile();
//void swap(struct IdStudent* , struct IdStudent*);
void swap_courses(ptr , ptr);

// linked list of student
struct student
{
    char name[20]; // student name
    int ID;// student ID
    int registerCourses;// The number of registered courses for the student
    poi next; // pointer to next node

};
struct student *studentList; // list for student
// linked list of courses
struct course
{
    char name[50];// course name
    char ID[10];// course ID
    int year;// year of student’s registration year
    int numOfStudent; // max number of students
    int registerStudent; // the number of registered students for the course

    ptr next; // pointer to next node
};
struct course *courseList;//  list for courses
// linked list of ID student
struct IdStudent
{
    int id; // student ID
    struct IdStudent *next; // pointer to next node
};
// linked list to connect students and courses
struct courseTostudent
{
    struct  IdStudent *student; // pointer to student ID in the struct IdStudent
    char ID_cor [10]; // course ID

};
struct courseTostudent course_studentList[25]; // array of linked list for the courses
struct courseTostudent watingList[25]; // array of linked list for the wating students
typedef struct courseTostudent*pos;

int main()
{
    courseList = NULL;
    studentList = NULL;
    int op;
    int stu_id;
    int course_index;
    FILE *f;
    f=fopen("Report.txt","w");
    while(op != 0)
    {

        printf("-------- Menu --------\n\n");
        printf("1. read lists from files\n");
        printf("2. Add a student to a course\n");
        printf("3. Remove a student from a course\n");
        printf("4. Remove a student from a waiting list of a course\n");
        printf("5. Write a report to file\n");
        printf("6. Print a report of the list of students sorted by student ID\n");
        printf("7. print a report of the list of courses sorted by course name\n");
        printf("8. Print a report of the list of courses that have less than 5 student\n");
        printf("9. Find course \n");
        printf("10. Find student \n");
        printf("0- Exit\n");
        printf("please choose an option from menu\n");

        scanf("%d",&op);

        if (op == 1) // Read files
            readFromFile();

        else  if (op == 2)// Add student
        {
            printf("Student id: ");
            scanf("%d", &stu_id);
            // for loop to print all courses ID&name

            for(int i = 0; i < 25; i++)
            {
                ptr course = findCourse(course_studentList[i].ID_cor);
                printf("%d- %s - %s\n", i+1, course->name, course->ID);
            }
            printf("Course number to add: ");
            scanf("%d", &course_index);
            //and check if we can add student to this course
            validation(stu_id, course_studentList[course_index-1].ID_cor);
        }
        else if (op == 3) // Remove student from course
        {
            printf("Student id: ");
            scanf("%d", &stu_id);
            poi student = findStudent(stu_id); // find student
            // Validate some conditions
            if(student == NULL)
            {
                printf("This student: [%d] does not exist in the list\n",stu_id);
                break;
            }
            if((student->registerCourses) == 0)
            {
                printf("This student: **%s** has not registered for any course\n", student->name);
                break;
            }
            printf("\n");
            // Print all courses registered by the student
            for(int i = 0; i < 25; i ++)
            {
                struct IdStudent *temp = course_studentList[i].student;
                while(temp != NULL)
                {
                    if(temp->id == student->ID)
                    {
                        printf("%d-%s\n",i+1,course_studentList[i].ID_cor);
                    }
                    temp = temp->next;
                }
            }
            //Read from user the course ID
            printf("Course ID: ");
            fflush(stdin);
            char cid[] = "";
            gets(cid);
            fflush(stdin);
            // check if the remove valid
            valid_Remove(student->ID, cid);
            //printf("-----------------------------------------------------------------\n");

        }
        else if (op == 4) // Remove student from waiting list
        {
            printf("Student id: ");
            scanf("%d", &stu_id);
            poi student = findStudent(stu_id); // find student
            if(student == NULL)
            {
                printf("This student: [%d] does not exist in the list\n",stu_id);
                break;
            }

            printf("\n");
            // Print all courses in waiting list registered by the student
            for(int i = 0; i < 25; i ++)
            {
                struct IdStudent *temp = watingList[i].student;
                while(temp != NULL)
                {
                    if(temp->id == student->ID)
                    {
                        printf("%d-%s\n",i+1, watingList[i].ID_cor);
                    }
                    temp = temp->next;
                }
            }
            printf("\nCourse ID: ");
            fflush(stdin);
            // Read course ID from user
            char cid[] = "";
            gets(cid);
            fflush(stdin);
            ptr c = findCourse(cid); // find course
            if(c == NULL)
            {
                printf("This course: [%s] does not exist in the course list\n",cid);
            }
            else
            {
                int index = getIndexWating(cid); // get index of course in waiting list
                struct IdStudent *temp = watingList[index].student;
                printf("%d\n",index);
                int exists = 0;
                //Find the student from waiting list of this course
                while(temp != NULL)
                {
                    if(temp->id == student->ID)

                        exists = 1;

                    temp = temp->next;
                }
                // If we found the student --> remove
                if(exists == 1)
                {
                    removeFromWatingList(student->ID, index);
                    printf("This student: **%s** has been removed from this course: [%s] successfully\n", student->name, c->name);
                }
                else
                {
                    printf("This student: **%s** is not in the waiting list of this course: [%s]\n", student->name, c->name);
                }
            }
        }
        else if (op == 6) // Print student list after sorted by student ID
        {

            printf("*******Report of student list sorted by student ID*******\n");
            fprintf(f,"*******Report of student list sorted by student ID*******\n");

            for (int i =0; i<25 ; i++)// to sort all the array [ course-student-list ]& print it
            {
                bubbleSort(i);
                printf("%d-***%s*** \n",i+1,course_studentList[i].ID_cor);
                fprintf(f,"%d-***%s*** \n",i+1,course_studentList[i].ID_cor);
                struct IdStudent *temp = course_studentList[i].student;
                while(temp != NULL)
                {
                    printf("- %d\n",temp->id);
                    fprintf(f,"- %d\n",temp->id);
                    temp = temp->next;
                }

            }
            fprintf(f,"--------------------------------------------------------------------\n");

        }
        else if (op == 7) // Print courses list after sorted by course name
        {
            Sort_courses();
            print_cource();

        }
        else if (op == 8) // Print all courses that have less than 5 student
        {
            ptr temp = courseList;
            printf("*****The courses that have less than 5 students*****\n\n");
            fprintf(f,"*****The courses that have less than 5 students*****\n\n");
            while(temp != NULL)
            {
                if (temp->registerStudent <= 5)
                    printf("- %s\n",temp->name);
                fprintf(f,"- %s\n",temp->name);
                temp = temp->next;
            }
            fprintf(f,"--------------------------------------------------------------------\n");

        }
        else if (op == 9)// Find course
        {
            printf("Course ID: ");
            char cid[] = "";
            scanf("%s",&cid);
            int index = findIndexOFcourse(cid);

            ptr temp =  findCourse(cid);
            struct IdStudent *c = course_studentList[index].student;

            if (temp != NULL)
            {
                printf("We found it [%s] \t [%s]\n",temp->ID,temp->name);
                fprintf(f,"We found it [%s] \t [%s]\n",temp->ID,temp->name);
                while(c != NULL)
                {
                    printf("- %d\n",c->id);
                    fprintf(f,"- %d\n",c->id);
                    poi s = findStudent(c->id);
                    printf("%s\n",s->name);
                    fprintf(f,"%s\n",s->name);
                    c= c->next;
                }

            }
            else
            {
                printf("Sorry this course not found!\n");
                fprintf(f,"Sorry this course not found!\n");
            }

        }
        else if (op == 10) // Find student
        {
            printf("Course ID: ");
            fflush(stdin);
            char cid[10] = "";
            gets(cid);
            fflush(stdin);


            printf("Student id: ");
            scanf("%d", &stu_id);

            int index = findIndexOFcourse(cid);
            struct IdStudent *temp = course_studentList[index].student;
int found =0;
            while(temp != NULL)
            {

                if(temp->id == stu_id)
                {
                    printf("We found it in this course: [%s] \n",cid);
                    fprintf(f,"We found it in this course: [%s] \n",cid);
                    found=1;
                }

                temp = temp->next;

            }
            if (found == 0){
            printf("NOT FOUND IT\n");
            fprintf(f,"NOT FOUND IT\n");
            }


        }
        else if (op == 5) // store to file

            storeToFile();
    }


    return 0;
}

void storeToFile()
{
    FILE *f;
    f=fopen("Report1.txt","w");


    fprintf(f,"*******Report of student list sorted by student ID*******\n");

    for (int i =0; i<25 ; i++)// to sort all the array [ course-student-list ]& print it
    {
        bubbleSort(i);
        fprintf(f,"%d-***%s*** \n",i+1,course_studentList[i].ID_cor);
        struct IdStudent *temp = course_studentList[i].student;
        while(temp != NULL)
        {
            fprintf(f,"- %d\n",temp->id);
            temp = temp->next;
        }

    }
    fprintf(f,"--------------------------------------------------------------------\n");
    ptr temp = courseList;
    fprintf(f,"*****The courses that have less than 5 students*****\n\n");
    while(temp != NULL)
    {
        if (temp->registerStudent <= 5)
            fprintf(f,"- %s\n",temp->name);
        temp = temp->next;
    }
    fprintf(f,"--------------------------------------------------------------------\n");


    fclose(f);

}

void readFromFile()
{
//Pointer to file
    FILE *fp;
    FILE *ff;
    //Open the file and read from it
    fp=fopen("Courses.txt","r");
    ff=fopen("Students.txt","r");
    // Array of char to store the file line by line;
    char lineByline[300];
    char line [300];
// Read courses from file and insert to course list
    while(!feof(fp))
    {
        ptr t;
        t= (ptr)malloc(sizeof(struct course));
        fgets(line,300, fp);
        char *name= strtok(line,",");
        strcpy(t->name,name);
        char *ID= strtok(NULL,",");
        strcpy(t->ID,ID);
        char *year= strtok(NULL,",");
        t->year= atoi(year);
        char *num= strtok(NULL,",");
        t->numOfStudent= atoi(num);
        t->next = NULL;
        if (findCourse(t->ID) != NULL)
            continue;
        insert_coruse(t);

    }
    fclose(fp);
    ptr temp = courseList;
// Store courses into waiting list& course-student List
    for (int i =0; i<25; i++)
    {
        strcpy(watingList[i].ID_cor,temp->ID );
        watingList[i].student = NULL;
        strcpy(course_studentList[i].ID_cor,temp->ID);
        course_studentList[i].student = NULL;
        temp = temp->next;

    }

// Read student from file and insert to student list
    while (!feof(ff))
    {
        poi temp1;
        temp1= (poi)malloc(sizeof(struct student));

        fgets(lineByline,300,ff);
        char *tokens;
        tokens =strtok(lineByline,"#");
        strcpy(temp1->name,tokens);
        tokens= strtok(NULL,"#");
        temp1->ID= atoi(tokens);

        if (findStudent(temp1->ID) != NULL)
            continue;

        insert_student(temp1);
        tokens= strtok(NULL,"\n");
        temp1->next = NULL;

        char *tok;// to store courses of student
        tok = strtok(tokens,"#");
        while (tok)
        {
            validation(temp1->ID, tok); // to check if the courses of student valid to added
            tok = strtok(NULL,"#");
        }
        printf("------------------------------------------\n\n");


    }
    fclose(ff);
    return;

}

void insert_coruse(ptr temp)
{
    ptr new_cource;
    new_cource = (ptr)malloc(sizeof(struct course));
    ptr last = courseList;
    strcpy(new_cource->name,temp->name);

    strcpy(new_cource->ID,temp->ID);

    new_cource->year=temp->year;
    new_cource->numOfStudent=temp->numOfStudent;
    new_cource->registerStudent=0;
    new_cource->next = NULL;


    if (courseList == NULL)
    {
        courseList = new_cource;
        return;
    }
    while (last->next != NULL)
        last = last->next;

    last->next = new_cource;



    return;

}

void insert_student(poi temp)
{

    poi new_student;
    new_student= (poi)malloc(sizeof(struct student));
    poi last = studentList;

    strcpy(new_student->name,temp->name);
    new_student->ID = temp->ID;
    new_student->registerCourses = 0;
    new_student->next = NULL;


    new_student->next = NULL;

    if (studentList == NULL)
    {
        studentList = new_student;
        return;
    }
    while (last->next != NULL)
        last = last->next;

    last->next = new_student;
    return;


}

// Find course by ID
ptr findCourse(char *cid)
{
    ptr temp = courseList;
    while(temp != NULL)
    {
        if (strcmp(temp->ID,cid)==0)

            return temp;
        temp = temp->next;

    }
    return NULL;
}

// Find id for student
poi findStudent(int id)
{
    poi s = studentList;
    while(s != NULL)
    {
        if (s->ID == id)
            return s ;
        s = s->next;
    }
    return NULL;
}

// Find index of course in the array of linked list (course_studentList)
int findIndexOFcourse(char *id)
{
    for (int i=0; i<25; i++)
    {
        if (strcmp(course_studentList[i].ID_cor,id) == 0)
            return i;
    }

    return -1; // if the course not found
}

// Check if the student already registered in course
int ifStudentExit(int s_id,int index)
{
    struct IdStudent * temp = course_studentList[index].student;
    while(temp != NULL)
    {
        if (temp->id == s_id)
            return 1 ;
        temp = temp->next;

    }
    return -1;// if the student not registered in the course

}

// Check if number of courses for student is full or not
int checkNumOFcourcses(poi stu)
{
    if(stu->registerCourses < 5)
        return 1;
    return -1;// if number of courses for student is full , not allowed to add any course
}

// Check if the course is full or not
int courseISfull(ptr course)
{
    if (course->numOfStudent > course->registerStudent)
        return -1;// if the course not full
    return 1;
}

// Check if a student year valid to register in specific course
int IFpossible(ptr course, poi student)
{
    if(course->year >= (int)(student->ID/10000))
        return 1;// if it possible to register
    return -1;
}

// Insert student to course
void registerStudent(int id, int index)
{
    struct IdStudent *new_node =( struct IdStudent*)malloc(sizeof(struct IdStudent));
    new_node->id = id;
    new_node->next = course_studentList[index].student;
    course_studentList[index].student = new_node;

}

// Get course index from waiting list
int getIndexWating(char *id)
{
    int i=0;
    while(i < 25)
    {
        if (strcmp(watingList[i].ID_cor,id) == 0)
            return i;
        i++;
    }
    return -1; // if the course not found
}

// check if the student is waiting ?
int stuISwating(int s_id, int index)
{
    // If we found it in waiting list for any course!
    struct IdStudent *temp = watingList[index].student;
    while(temp != NULL)
    {
        if (temp->id == s_id)
            return 1;
        temp = temp->next;
    }
    return -1; // the student not waiting for any course


}

// Check if the remove is valid
void valid_Remove(int s_id, char *c_id)
{
    ptr course = findCourse(c_id);
    if (course == NULL)
    {
        printf("This course: [%s] dose not exist\n",c_id);
        return;
    }
    poi stu = findStudent(s_id);
    if (stu == NULL)
    {
        printf("This student: [%d] dose not exist\n",s_id);
        return;
    }
    int index = findIndexOFcourse(c_id);
    if (index == -1)
    {
        printf("Error! the course [%s] not found \n",c_id);
        return;
    }
    if (ifStudentExit(s_id,index))
    {
        remove_stuFromCor(s_id,index); // remove student from course
        stu->registerCourses--; // decrease number of registered courses for student
        course->registerStudent--; // decrease number of registered student for course
        printf("This student: [%s] has removed from this course: [%s] successfully \n",stu->name,course->name);
        removeFromWatingList(s_id,index); // remove the student from waiting list
        return;
    }
    else
    {
        printf("This student: [%s] is not registered in this course: [%s] \n",stu->name,course->name);
        return;
    }
}

// Remove student from course from array of courses
void remove_stuFromCor(int s_id, int index)
{
    struct IdStudent *temp = course_studentList[index].student;
    struct IdStudent *prevous;
    if (temp != NULL && temp->id == s_id)
    {
        course_studentList[index].student = temp->next;
        free(temp);
        return;

    }
    while(temp != NULL && temp->id != s_id)
    {
        prevous = temp;
        temp = temp->next;
    }
    if (temp == NULL)
        return;
    prevous->next = temp->next;
    free(temp);


}

// Remove student from waiting list
void removeFromWatingList(int s_id,int index)
{
    struct IdStudent *temp = watingList[index].student,*prevous;
    if (temp != NULL && temp->id == s_id)
    {
        watingList[index].student = temp->next;
        free(temp);
        return;
    }
    while(temp != NULL && temp->id != s_id)
    {
        prevous = temp;
        temp = temp->next;
    }
    if (temp == NULL)
        return;
    prevous->next = temp->next;
    free(temp);

}

// Check if valid to insert in waiting list
int validationWatingList(int s_id,int index)
{
    poi stu = findStudent(s_id);
    if (stu == NULL)
        return -1; // If the student not found
    ptr cor = findCourse(watingList[index].ID_cor);
    if (cor == NULL)
        return -1; // If the course not found
    if (IFpossible(cor,stu) == -1)
        return -1; // If the student not allowed to add this course; because his/her year
    if (checkNumOFcourcses(stu) == -1)
        return -1; // If the student not allowed to add any course
    if (courseISfull(cor) == -1)
        return -1; // If the course is full number of student
    if (ifStudentExit(s_id,index))
        return -1; // If the student already registered in this course
    return 1;// If all conditions are valid
}

// Check if the student valid to added to the course
void validation(int s_id, char *c_id)
{
    poi student = findStudent(s_id);
    if(student == NULL)
    {
        printf("This student: - %d does not be exist !!\n\n",s_id);
        return;
    }
    ptr course = findCourse(c_id);
    if(course == NULL)
    {
        printf("This course: %s is not available!\n\n",c_id);
        return;
    }

    if(IFpossible(course, student) == -1)
    {
        printf("This student: %s is not allowed to register in this course: %s because of his/her registration year\n\n", student->name, course->name);
        return;
    }
    if(checkNumOFcourcses(student) == -1)
    {
        printf("This student: %s is not allowed to register since reached the maximum number of courses registered\n\n", student->name);
        return;
    }

    if(courseISfull(course) == 1)
    {
        printf("This student: %s can't register in this course: %s right not because the course is full\n\n", student->name, course->name);
        printf("Do you want to register in the waiting list (y)? ");
        char choice = getch();
        printf("\n");
        if(choice == 'y' || choice == 'Y')
        {
            int Waiting_index = getIndexWating(c_id); // get index of course from waiting list
            if(Waiting_index == -1)
            {
                printf("Error! this course: %s not found\n\n",c_id);
                return;
            }
            if(stuISwating(s_id, Waiting_index) == 1)
            {
                printf("This student: %s is already register in waiting list of this course: %s\n\n", student->name, course->name);
                return;
            }
            ADD_waitingList(s_id, Waiting_index);
            printf("This student: %s Registered in the waiting list of the course: %s successfully\n\n", student->name, course->name);
            return;
        }
        else
            return;
    }

    int index = findIndexOFcourse(c_id);
    if(index == -1)
    {
        printf("Error! this course not found\n");
        return;
    }
    if(ifStudentExit(student->ID, index) == 1)
    {
        printf("%s is already registered in %s\n", student->name, course->name);
        return;
    }
    registerStudent(s_id, index);
    student->registerCourses++;
    course->registerStudent++;
    printf("This student %s added to this course %s successfully\n",student->name, course->name);
}

// add student to waiting list
void ADD_waitingList(int s_id, int index)
{
    struct IdStudent *temp = (struct IdStudent*)malloc(sizeof(struct IdStudent));
    temp->id = s_id;
    temp->next = watingList[index].student;
    watingList[index].student = temp;
    printf("%d %d\n",s_id,index);

}

// print report of courses
void print_cource()
{
    ptr p;
    p=courseList;
    printf("the list of courses :\n");
    //print the list while the next pointer do not point to NULL
    while(p->next != NULL)
    {
        int index = getIndexWating(p->ID);
        int num=0;
        struct IdStudent *temp = watingList[index].student;
        while(temp != NULL)
        {
            num++;
            temp = temp->next;
        }

        printf("Course name: %s\nCourse ID: %s\nYear: %d\nnumber of max student: %d\nNumber of registered students: %d\n", p->name,p->ID,p->year,p->numOfStudent,p->registerStudent);
        printf("Number of student in waiting list: [ %d ] \n",num);
        printf("-------------------------------------------------------------\n");
        p = p->next;

    }
    printf("\n");




}

// sort student according to ID
void bubbleSort(int index )
{
    int swapped, i;
    struct IdStudent *ptr1;
    struct IdStudent *lptr = NULL;

    /* Checking for empty list */
    if ( course_studentList[index].student == NULL)
        return;

    do
    {
        swapped = 0;
        ptr1 = course_studentList[index].student ;

        while (ptr1->next != lptr)
        {
            if (ptr1->id > ptr1->next->id)
            {
                swap(ptr1, ptr1->next); // call function swap
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}

/* function to swap data ID of two nodes a and b*/
void swap(struct IdStudent *a, struct IdStudent *b)
{
    int temp = a->id;
    a->id = b->id;
    b->id = temp;
}

// sort courses according to course name
void Sort_courses()
{
    int swapped, i;
    ptr ptr1;
    ptr lptr = NULL;

    /* Checking for empty list */
    if ( courseList == NULL)
        return;

    do
    {
        swapped = 0;
        ptr1 = courseList ;

        while (ptr1->next != lptr)
        {
            if (strcmp(ptr1->name, ptr1->next->name) > 0 )
            {
                swap_courses(ptr1, ptr1->next); // call function swap_courses
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}

/* function to swap course data of two nodes a and b*/
void swap_courses(ptr a, ptr b)
{
    ptr temp = (ptr)malloc(sizeof(struct course)) ;
    strcpy(temp->ID,a->ID);
    strcpy(temp->name,a->name);
    temp->numOfStudent = a->numOfStudent;
    temp->registerStudent = a->registerStudent;
    temp->year = a->year;

    strcpy(a->ID,b->ID);
    strcpy(a->name,b->name);
    a->numOfStudent = b->numOfStudent;
    a->registerStudent = b->registerStudent;
    a->year = b->year;

    strcpy(b->ID,temp->ID);
    strcpy(b->name,temp->name);
    b->numOfStudent = temp->numOfStudent;
    b->registerStudent = temp->registerStudent;
    b->year = temp->year;
}
