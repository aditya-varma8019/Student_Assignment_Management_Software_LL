#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct Student_record {
    int student_id;
    char name[50];
    int group_partner_id;
    int group_id;
    int assignment_id;
    char assignment_name[50];
    char deadline[20];
    char status_of_assignment[20];//(submitted/not-submitted/evaluated
    float offline_evaluation_marks;
    float viva_marks;
    struct Student_record* next;
}Student_record;

typedef struct Assignment {
    int assignment_id;
    char topic_name[50];
    char status[20];//(declared/due-date-over/evaluated)
    struct Assignment_record* group_list;
    struct Assignment* next;
}Assignment;

typedef struct Assignment_record {
    int group_id;
    char status[20];//(declared/submitted/not-submitted/evaluated) 
    float viva_marks;
    float offline_marks;
    struct Assignment_record* next;
}Assignment_record;

Student_record* student_list=NULL;
Assignment* assignment_list=NULL;

//1-----------------------------------------------------------------------------------------------------

void insert_student_record()
{
    Student_record* new_student=(Student_record*)malloc(sizeof(Student_record));
    printf("Enter student ID: ");
    scanf("%d", &new_student->student_id);
    printf("Enter student name: ");
    scanf("%s", new_student->name);
    printf("Enter group partner id: ");
    scanf("%d", &new_student->group_partner_id);
    printf("Enter group ID: ");
    scanf("%d", &new_student->group_id);
    printf("Enter assignment id: ");
    scanf("%d", &new_student->assignment_id);
    printf("Enter assignment name: ");
    scanf("%s", new_student->assignment_name);
    printf("Enter deadline: ");
    scanf("%s", new_student->deadline);
    printf("Enter status of assignment (submitted/not submitted/evaluated): ");
    scanf("%s", new_student->status_of_assignment);
    printf("Enter offline evaluation marks: ");
    scanf("%f", &new_student->offline_evaluation_marks);
    printf("Enter viva marks: ");
    scanf("%f", &new_student->viva_marks);
    new_student->next = NULL;

    if (student_list == NULL) {
        student_list = new_student;
    } else {
        Student_record *current_student = student_list;
        while (current_student->next != NULL) {
            current_student = current_student->next;
        }
        current_student->next = new_student;
    }

    Assignment *current_assignment = assignment_list;
    while(current_assignment->next!=NULL && current_assignment->next->assignment_id!=new_student->assignment_id)
    {
        current_assignment=current_assignment->next;
    }
    if(current_assignment->next==NULL)
    {
        Assignment* temp;
        temp=(Assignment*)malloc(sizeof(Assignment));
        temp->assignment_id=new_student->assignment_id;
        //printf("Enter the status of the assignment");
        strcpy(temp->status,new_student->status_of_assignment);
        //strcpy(temp->status,"declared");
        strcpy(temp->topic_name,new_student->assignment_name);
        temp->next=NULL;
        temp->group_list=NULL;
        current_assignment->next=temp;
    }
    else
    {
        Assignment_record* new_group=current_assignment->group_list;
        while(new_group!=NULL && new_group->group_id!=new_student->group_id)
        {
            new_group=new_group->next;
        }
        if(new_group==NULL)
        {
            new_group=(Assignment_record*)malloc(sizeof(Assignment_record));
            new_group->group_id=new_student->group_id;
            new_group->offline_marks=new_student->offline_evaluation_marks;
            new_group->viva_marks=new_student->viva_marks;
            strcpy(new_group->status,new_student->status_of_assignment);
            new_group->next=NULL;
        }
        else
        {
            new_group->offline_marks+=new_student->offline_evaluation_marks;
            new_group->viva_marks+=new_student->viva_marks;
            strcpy(new_group->status,new_student->status_of_assignment);
        }
    }
    //maybe changes should be made for group_list also??
}

//2--------------------------------------------------------------------------------------------------------

void insert_assignment_record()
{
    Assignment* new_assignment = (Assignment*)malloc(sizeof(Assignment));
    printf("Enter assignment id :");
    scanf("%d",&new_assignment->assignment_id);
    printf("Enter topic name :");
    scanf("%s",new_assignment->topic_name);
    printf("Enter the status of the assignment(declared/due-date-over/evaluated)");
    scanf("%s",new_assignment->status);
    new_assignment->next=NULL;
    new_assignment->group_list=NULL;

    Assignment* curr=assignment_list;
    if(curr==NULL)
    {
        assignment_list=new_assignment;
    }
    else
    {
        while(curr->next!=NULL && curr->next->assignment_id!=new_assignment->assignment_id)
        {
            curr=curr->next;
        }
        if(curr->next==NULL)
        {
            curr->next=new_assignment;
        }
        else
        {
            Assignment_record* curr_rec=curr->next->group_list;
            if(curr_rec==NULL)
            {
                curr_rec=(Assignment_record*)malloc(sizeof(Assignment_record));
                strcpy(curr_rec->status,curr->next->status);
                curr_rec->next=NULL;
            }
            else
            {
                while(curr_rec->next!=NULL)
                {
                    curr_rec=curr_rec->next;
                }
                Assignment_record* new_rec=(Assignment_record*)malloc(sizeof(Assignment_record));
                new_rec->next=NULL;
                strcpy(new_rec->status,curr->next->status);
            }
        }
    }

    Student_record* curr_student=student_list;
    while(curr_student!=NULL)
    {
        if(curr_student->assignment_id==new_assignment->assignment_id)
        {
            Assignment_record* new_record=(Assignment_record*)malloc(sizeof(Assignment_record));
            new_record->group_id=curr_student->group_id;
            strcpy(new_record->status,"declared");
            new_record->viva_marks=0.0;
            new_record->offline_marks=0.0;
            new_record->next=NULL;

            Assignment_record* temp_record=new_assignment->group_list;
            if(temp_record==NULL)
            {
                new_assignment->group_list=new_record;
            }
            else
            {
                while(temp_record->next!=NULL)
                {
                    temp_record=temp_record->next;
                }
                temp_record->next=new_record;
            }
        }
        curr_student=curr_student->next;
    }
    
}
//3(a)------------------------------------------------------------------------

void submit_assignment(int a_id,int g_id)
{
    int flag=0;
    Assignment* curr_ass=assignment_list;
    while(curr_ass!=NULL && flag==0)
    {
        if(curr_ass->assignment_id==a_id)
        {
            flag=1;
            strcpy(curr_ass->status,"submitted");
        }
        curr_ass=curr_ass->next;
    }
    if(flag==0)
    {
        printf("Assignment is not found\n");
        return;
    }
    flag=1;
    Student_record* curr_s=student_list;
    while(curr_s!=NULL && flag>=0)
    {
        if(curr_s->group_id==g_id && curr_s->assignment_id==a_id)
        {
            if(strcmp(curr_s->status_of_assignment,"submitted")==0)
            {
                printf("Assignment is already submitted!!\n");
                return;
            }
            else
            {
                flag--;
                strcpy(curr_s->status_of_assignment,"submitted");
                printf("Assignment is submitted successfully!!\n");
            }
        }
        curr_s=curr_s->next;
    }
    if(flag>=0)
    {
        printf("Assignment not found for the given group\n");
        return;
    }
}
//3(b)--------------------------------------------------------------------

void evaluate_assignment(int a_id,int g_id)
{
    int flag=0;
    Assignment* curr_ass=assignment_list;
    while(curr_ass!=NULL )
    {
        if(curr_ass->assignment_id==a_id)
        {
            flag=1;
            strcpy(curr_ass->status,"evaluated");
            Assignment_record* curr_rec=curr_ass->group_list;
            while(curr_rec!=NULL)
            {
                strcpy(curr_rec->status,"evaluated");
                curr_rec=curr_rec->next;
            }
            break;
        }
        curr_ass=curr_ass->next;
    }
    if(flag==0)
    {
        printf("Assignment is not found\n");
        return;
    }
    flag=0;
    // Assignment_record* temp=curr_ass->group_list;
    // while(temp!=NULL)
    // {
    //     if(temp->group_id==g_id && strcmp(temp->status,"submitted")==0)
    //     {
    //         flag=1;
    //         printf("Enter the offline marks of %s: ",temp->name);
    //         scanf("%f",&temp-);
    //         printf("Enter the viva marks of %s: ",temp->name);
    //         scanf("%f",&temp->viva_marks);
    //         strcpy(curr_s->status_of_assignment,"evaluated");
    //     }
    //     temp=temp->next;
    // }
    Student_record* curr_s=student_list;
    while(curr_s!=NULL && flag==0)
    {
        if(curr_s->group_id==g_id && curr_s->assignment_id==a_id)
        {
            if(strcmp(curr_s->status_of_assignment,"submitted")!=0)
            {
                printf("Assignment is not submitted!!\n");
                return;
            }
            else
            {
                flag=1;
                printf("Enter the offline marks of %s: ",curr_s->name);
                scanf("%f",&curr_s->offline_evaluation_marks);
                printf("Enter the viva marks of %s: ",curr_s->name);
                scanf("%f",&curr_s->viva_marks);
                strcpy(curr_s->status_of_assignment,"evaluated");
            }
        }
        curr_s=curr_s->next;
    }
    
    if(flag==0)
    {
        printf("Assignment not found for the given group\n");
        return;
    }
    else
    {
        printf("Assignment evaluated successfully!!!\n");
    }
}
//4------------------------------------------------------------------------------

void printDeclaredNotEvaluatedAssignments() {
    printf("List of assignments that are declared but not evaluated:\n");
    Assignment *curr_assignment = assignment_list;
    while (curr_assignment != NULL) {
        if (strcmp(curr_assignment->status ,"declared")==0) {
            printf("\nAssignment ID: %d, Topic Name: %s",curr_assignment->assignment_id, curr_assignment->topic_name);
        }
        curr_assignment = curr_assignment->next;
    }
}
//5------------------------------------------------------------------------------

void printOverdueStudentGroups(char *date) {
    int t_d,t_m,t_y;
    int temp=0;
    for(int i=0;i<10;i++)
    {
        if(date[i]!='/')
        {
            temp=temp*10+(date[i]-'0');
        }
        else
        {
            if(i==2)
            {
                t_d=temp;
            }
            else if(i==5)
            {
                t_m=temp;
            }
            temp=0;
        }
    }
    t_y=temp;
    printf("List of student groups who have not submitted the assignments even if the due-date is over:\n");
    Student_record *curr_student = student_list;
    while (curr_student != NULL) {
        int dd,dm,dy;
        int temp=0;
        for(int i=0;i<10;i++)
        {
            if(curr_student->deadline[i]!='/')
            {
                temp=temp*10+(curr_student->deadline[i]-'0');
            }
            else
            {
                if(i==2)
                {
                    dd=temp;
                }
                if(i==5)
                {
                    dm=temp;
                }
                temp=0;
            }
        }
        dy=temp;
       if(t_y>dy)
       {
             printf("Group ID: %d, Assignment Name/ID: %s\n", curr_student->group_id, curr_student->assignment_name);
       }
       else if(t_m>dm && t_y==dy)
       {
             printf("Group ID: %d, Assignment Name/ID: %s\n", curr_student->group_id, curr_student->assignment_name);
       }
       else if(t_d>dd && t_m==dm && t_y==dy)
       {
             printf("Group ID: %d, Assignment Name/ID: %s\n", curr_student->group_id, curr_student->assignment_name);
       }
       curr_student=curr_student->next;
    }
}
//6------------------------------------------------------------------------------

void printPendingEvaluations(int a_id)
{
    Assignment* curr_ass=assignment_list;
    while(curr_ass!=NULL && curr_ass->assignment_id!=a_id)
    {
        curr_ass=curr_ass->next;
    }
    if(curr_ass==NULL)
    {
        printf("Assignment is not found!!\n");
        return;
    }
    Assignment_record* temp=curr_ass->group_list;
    if(temp==NULL)
    {
        printf("There are no student groups for the given assignment!!!\n");
        return;
    }
    while(temp!=NULL)   
    {
        if(strcmp(temp->status,"submitted")==0 && temp->viva_marks!=-1.0 && temp->offline_marks!=-1.0)
        {
            printf("Evaluation of students of Group id: %d is not yet taken\n",temp->group_id);
        }
        if(strcmp(temp->status,"submitted")==0 && temp->viva_marks==-1.0 && temp->offline_marks!=-1.0)
        {
            printf("Viva of students group id :%d is yet to be taken\n",temp->group_id);
        }
        if(strcmp(temp->status,"submitted")==0 && temp->viva_marks!=-1.0 && temp->offline_marks==-1.0)
        {
            printf("Offline evaluation of students group id :%d is yet to be taken\n",temp->group_id);
        }
        temp=temp->next;
    }
}


void printPendingEvaluationsAll(int a_id)
{
    Assignment* curr_ass=assignment_list;
    while(curr_ass!=NULL && curr_ass->assignment_id!=a_id)
    {
        Assignment_record* temp=curr_ass->group_list;
        if(temp==NULL)
        {
            printf("There are no student groups for the given assignment!!!\n");
            return;
        }
        while(temp!=NULL)
        {
            if(strcmp(temp->status,"submitted")==0 && temp->viva_marks!=-1.0 && temp->offline_marks!=-1.0)
            {
                printf("Evaluation of students of Group id: %d is not yet taken\n",temp->group_id);
            }
            if(strcmp(temp->status,"submitted")==0 && temp->viva_marks==-1.0 && temp->offline_marks!=-1.0)
            {
                printf("Viva of students group id :%d is yet to be taken\n",temp->group_id);
            }
            if(strcmp(temp->status,"submitted")==0 && temp->viva_marks!=-1.0 && temp->offline_marks==-1.0)
            {
                printf("Offline evaluation of students group id :%d is yet to be taken\n",temp->group_id);
            }
            temp=temp->next;
        }
        curr_ass=curr_ass->next;
    }
}
//7-----------------------------------------------------------------------------------------------------------

void swap(Assignment_record *a, Assignment_record *b) 
{ 
    int temp1= a->offline_marks;
    int temp2=a->viva_marks ;
    a->offline_marks=b->offline_marks;a->viva_marks = b->viva_marks; 
    b->offline_marks=temp1;b->viva_marks = temp2; 
} 

void bubbleSort(Assignment_record  *start) 
{ 
    int swapped, i; 
    Assignment_record  *ptr1; 
    Assignment_record  *lptr = NULL; 
  
    if (start == NULL) 
        return; 
  
    do
    { 
        swapped = 0; 
        ptr1 = start; 
  
        while (ptr1->next != lptr) 
        { 
            if (ptr1->offline_marks + ptr1->viva_marks < ptr1->next->offline_marks + ptr1->next->viva_marks) 
            { 
                swap(ptr1, ptr1->next); 
                swapped = 1; 
            } 
            ptr1 = ptr1->next; 
        } 
        lptr = ptr1; 
    } 
    while (swapped); 
} 
  


void studentGroupsDecreasingOfMarks(int a_id)
{
    Assignment* curr_ass=assignment_list;
    while(curr_ass!=NULL && curr_ass->assignment_id!=a_id)
    {
        curr_ass=curr_ass->next;
    }
    if(curr_ass==NULL)
    {
        printf("Entered assignment is not found!!\n");
        return;
    }
    Assignment_record* curr_rec=curr_ass->group_list;
    bubbleSort(curr_rec);
    while(curr_rec!=NULL)
    {
        printf("The group id is : %d,offline marks is: %f,viva marks is : and total marks are: %f %f\n",curr_rec->group_id,curr_rec->offline_marks,curr_rec->viva_marks);
        curr_rec=curr_rec->next;
    }
}
//8-------------------------------------------------------------------------------

void groupsWithTopMarks()
{
    Assignment* curr_ass=assignment_list;
    while(curr_ass!=NULL)
    {
        float max_marks=-1;
        Assignment_record* curr_rec=curr_ass->group_list;
        while(curr_rec!=NULL)
        {
            if(max_marks<(curr_rec->offline_marks+curr_rec->viva_marks))
            {
                max_marks=(curr_rec->offline_marks+curr_rec->viva_marks);
            }
            curr_rec=curr_rec->next;
        }
        // if(max_marks==-1)
        // {
        //     continue;
        // }
        curr_rec=curr_ass->group_list;
        while(curr_rec!=NULL)
        {
            if(max_marks==(curr_rec->offline_marks+curr_rec->viva_marks))
            {
                printf("The group of students of group id: %d,recived the highest marks of: %.2f in the assignment of assignmnet id: %d\n",curr_rec->group_id,max_marks,curr_ass->assignment_id);
            }
            curr_rec=curr_rec->next;
        }
        curr_ass=curr_ass->next;
    }
}

void printStudentList()
{
    Student_record* a=student_list;
    
    while(a!=NULL)
    {
        printf("%d %s %d %d %s %d %s %s %.2f %.2f\n",
        a->student_id, a->name, a->group_partner_id,
        a->group_id, a->assignment_name,a->assignment_id,a->deadline,
        a->status_of_assignment, a->offline_evaluation_marks, a->viva_marks);
        a=a->next;
    }
}

void printAssignmentList()
{
    Assignment* b=assignment_list;
    Assignment_record*c;
    while(b!=NULL)
        {
            printf("%d %s %s %d",b->assignment_id,b->topic_name,b->status);
            c=b->group_list;
            while(c!=NULL)
            {
                printf("%d %s %.2f %.2f \n",c->group_id,c->status,c->offline_marks,c->viva_marks);
                c=c->next;
            }
            b=b->next;
        }
}

void function()
{

    FILE* fp;
char line[200];
int numStudents,numAssignments;
fp = fopen("list.txt", "r");
if (fp == NULL)  
{
    printf("Error opening file.");
    exit(1);
}

fgets(line, sizeof(line), fp);
sscanf(line,"%d %d",&numStudents,&numAssignments);

for(int i=0;i<numStudents;i++)
{
    Student_record* student=(Student_record*)malloc(sizeof(Student_record));
    fgets(line, sizeof(line), fp);
    sscanf(line,"%d %s %d %d %s %d %s %s %f %f",
           &student->student_id, student->name, &student->group_partner_id,
           &student->group_id, student->assignment_name,&student->assignment_id,student->deadline,
           student->status_of_assignment, &student->offline_evaluation_marks, &student->viva_marks);
    student->next = student_list;
    student_list = student;
}

for(int j=0;j<numAssignments;j++)
{
    Assignment* assignment = (Assignment*)malloc(sizeof(Assignment));
    int numGroups;
    fgets(line, sizeof(line), fp);
    sscanf(line,"%d %s %s %d",&assignment->assignment_id,assignment->topic_name,assignment->status,&numGroups);
    assignment->group_list=NULL;
    for(int k=0;k<numGroups;k++)
    {
        Assignment_record* group=(Assignment_record*)malloc(sizeof(Assignment_record));
        fgets(line, sizeof(line), fp);
        sscanf(line,"%d %s %f %f",&group->group_id,group->status,&group->offline_marks,&group->viva_marks);
        group->next=assignment->group_list;
        assignment->group_list=group;
    }
    assignment->next=assignment_list;
    assignment_list=assignment;
}
fclose(fp);

    
    int choice;
    do {
        
        
        int a_id,g_id;
        char date[10];
        printf("\n1. Insert student record\n2. Insert assignment record\n3. Submit assignment\n4. Evaluate assignment\n5. Print declared assignments\n6. Print missing submissions \n7. Print pending evaluations\n8. Print assignment results\n9. Print top performers\n10. Print student list\n11. Print Assignment List\n0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                insert_student_record();
                break;
            case 2:
                insert_assignment_record();
                break;
            case 3:
                
                printf("Enter the assignment id and group id: ");
                scanf("%d %d",&a_id,&g_id);
                submit_assignment(a_id,g_id);
                break;
            case 4:
                
                printf("Enter the assignment id and group id: ");
                scanf("%d %d",&a_id,&g_id);
                evaluate_assignment(a_id,g_id);
                break;
            case 5:
                printDeclaredNotEvaluatedAssignments();
                break;
            case 6:
                printf("Enter today's date: ");
                scanf("%s",date);
                printOverdueStudentGroups(date);
                break;
            case 7:
                printf("Enter the assignment id: ");
                scanf("%d",&a_id);
                printPendingEvaluations(a_id);
                break;
            case 8:
                printf("Enter the assignment id: ");
                scanf("%d",&a_id);
                studentGroupsDecreasingOfMarks(a_id);
                break;
            case 9:
                groupsWithTopMarks();
                break;
            case 10:
                printStudentList();
                break;
            case 11:
                printAssignmentList();
                break;        
            case 0:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    } while (choice != 0);
}



int main() {
    int option,i=0;
    char name[50];
    char password[50];
    printf("Login as :\n1. Student \n2. Teacher\nEnter your choice :");
    scanf("%d",&option);
    printf("\nYou have choosen %d\n",option);
    switch (option)
    {
    case 1:printf("---------------------Student's Portal ---------------------------------\n");
            printf("Enter your name :");
            scanf("%s",name);
            while (name[i]) {
                name[i] = tolower(name[i]);
                i++;
            }
            printf("\nEnter your password (all in lowercase):");
            scanf("%s",password);
            if(strcmp(password,name)==0)
            {
                printf("Login Successful!!\n");
                function();
            }
            else
            {
                printf("Incorrect password!!\n");
                break;
            }
            
        break;
    case 2:printf("---------------------Teacher's Portal ---------------------------------\n");
            printf("Enter your name :");
            scanf("%s",name);
            while (name[i]) {
                name[i] = tolower(name[i]);
                i++;
            }
            printf("\nEnter your password (all in lowercase):");
            scanf("%s",password);
            if(strcmp(password,name)==0)
            {
                printf("\nLogin Successful!!");
            }
            else
            {
                printf("\nIncorrect password!!");
                break;
            }
            function();    
    
    default: printf("\nEntered choice is invalid!!");
            break;
    }
    
    return 0;
}
