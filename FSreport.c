#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <math.h>


// files
char nextLevelFiles[1000][1000];
char origNameFiles[1000][1000];


// folders
char oldLevelFolders[1000][1000];
char nextLevelFolders[1000][1000];
char origNameFolders[1000][1000];
char origOldNameFolders[1000][1000];
char dirName[1000][1000];

// integers
int levelCount = 1, inodeLevelCount = 1, indexValueFolder = 0, indexValueFiles = 0, indexValueOldFolder = 0, runThroughAllDir= 0;
int folderIndex = 0, folderTemp = 0, currIndex = 0, totalFolders = 0, structCounter = 0;
int numberOfFoldersInLevel[1000], levelNumber = 0, inodeNo[1000], bytes[1000], fileSize[1000], blockSize[1000], levelId[1000], inodeLevelArray[1000], inodeFolderNumber = 0;

void treeReport(char *path);
void getCurrentLevelInfoFolders(char * path, int i, int placement);
void getCurrentLevelInfoFiles(char * path, int i, int placement);
void firstLevelValues(char * path, int i, int placement);
void inodeReport(char * path);
void stringFunction(char * path, struct dirent *filePtr);
void newLevel(char * path, int placement);


int main(int argc, char* argv[]) // main
{
  if(argc != 3)
  {
    printf("Error with command line input. Needs more entries.\n");
    exit(0);
  }

  char path[100];
  char *symlinkpath = ".";
  char actualpath [PATH_MAX + 1];
  char *ptr;
  ptr = realpath(symlinkpath, actualpath); // get path string 
  

  char * newPath = malloc(sizeof(char)* 1000);


  if(strcmp(ptr, argv[2]) == 0) // in current directory
  {
      strcpy(path, ".");
  }
  else // not in current directory
  {
    int length = strlen(ptr);

    int lengthInput = strlen(argv[2]);

    sprintf(newPath, "%s", argv[2] + length);

    strcpy(path, ".");
    strcat(path, newPath);

  }

  bool reportOccurs = false;

  struct dirent *filePtr;
  DIR *dir = opendir(path);
  int i = 0;

  int userInputLength = strlen(argv[2]);

  int loopTotal = strlen(ptr);
  int pathCheck = -1;
  int numOfDir = 0, prevNumber = 0, m = 0;

  char newChangeOfPath[1000];
  int changeOfPathIndex = 0;


  if (!dir) // path checkers
  {

      for(m = 0; m < loopTotal; m++)
      {

        if(ptr[m] != argv[2][m])
        {
          pathCheck = 0;
          break;

        }
      }

      if(ptr[m-1] ==  '/')
      {
        numOfDir++;
      }

    prevNumber = m;

    for(m = prevNumber; m < loopTotal; m++)
    {

      newChangeOfPath[changeOfPathIndex] = argv[2][m];
      changeOfPathIndex++;

      if(ptr[m] ==  '/')
      {
        numOfDir++;
      }

    }
    if(numOfDir == 0)
    {
      numOfDir = 1;
    }

    strcpy(path,"");
    int currCounter = 0;

    while(currCounter < numOfDir)
    {
      strcat(path,"../");
      currCounter++;
    }
    strcat(path, newChangeOfPath);

    int tempNum = userInputLength;

    dir = opendir(path);

    strcpy(newChangeOfPath,"");

    if(!dir)
    {
      if(argv[2][userInputLength - 1] == '/')
      {
        tempNum = tempNum - 1;
      }
      for(m = tempNum - 1; m >= 0; m--)
      {
        int value = m + 1, x = 0;

        if(argv[2][m] == '/')
        {
          for(x = 0; x < tempNum - m; x++)
          {
            newChangeOfPath[x] = argv[2][value];
            value++;
          }
          for(int p = x; p < tempNum; p++)
          {
            newChangeOfPath[p] = '\0';
          }

          break;

        }
      }

      strcpy(path,"");

      currCounter = 0;

      while(currCounter <= numOfDir)
      {
        strcat(path,"../");
        currCounter++;
      }

      strcat(path, newChangeOfPath);

    }

  }

  dir = opendir(path);
  if(!dir) // last check
  {
    printf("Directory not found.\n");
    return 0;
  }

  // do reports
  if (strcmp(argv[1],"-tree") == 0)
  {

    printf("\nFile System Report: Tree Directory Structure\n");
    printf("\nLevel %d: %s", levelCount, argv[2]);

    treeReport(path); // do tree report
    reportOccurs = true;

  }
  else if (strcmp(argv[1],"-inode") == 0)
  {

    printf("\nFile System Report: Inodes\n");
    printf("\nLevel %d Inodes: %s\n", inodeLevelCount, argv[2]);



    inodeReport(path); // do inode report
    reportOccurs = true;

  }

  if(!reportOccurs)
  {
    printf("No reports have occcured due to incorrect command line input.\n"); // neither tree or inode was inputted in command line
  }

  return 0;
}
void inodeReport(char * path) // do inode report
{

  struct dirent *filePtr;
  DIR *dir = opendir(path);
  int i = 0;

  if (!dir) 
  {
      printf("\nNo directory.\n");
      return; 
  }


  while ((filePtr = readdir(dir)) != NULL)
  {
      stringFunction(path, filePtr);
  }


  firstLevelValues(path, i, 1); // the one signifies it'll output inode information

  closedir(dir);

}




void treeReport(char *path)
{
  struct dirent *filePtr;
  DIR *dir = opendir(path);
  int i = 0;


  if (!dir) 
  {
      printf("\nNo directory.\n");
      return; 
  }


  while ((filePtr = readdir(dir)) != NULL)
  {
      stringFunction(path, filePtr);
  }


  for (i = 0; i < indexValueFolder; i++) // bubble sort folders alphabetically
  {
      for (int j = 0; j < indexValueFolder - 1; j++)
      {

          if (strcmp(origNameFolders[j], origNameFolders[j + 1]) > 0) 
          {
              char* temp;
              temp = (char*)calloc(30, sizeof(char));
              strcpy(temp, origNameFolders[j]);
              strcpy(origNameFolders[j], origNameFolders[j + 1]);
              strcpy(origNameFolders[j + 1], temp);

              temp = (char*)calloc(30, sizeof(char));
              strcpy(temp, nextLevelFolders[j]);
              strcpy(nextLevelFolders[j], nextLevelFolders[j + 1]);
              strcpy(nextLevelFolders[j + 1], temp);
          }
      }

  }

  for (i = 0; i < indexValueFiles; i++) // bubble sort files alphabetically 
  {
      for (int j = 0; j < indexValueFiles - 1; j++)
      {

          if (strcmp(origNameFiles[j], origNameFiles[j + 1]) > 0) 
          {
              char* temp;
              temp = (char*)calloc(30, sizeof(char));
              strcpy(temp, origNameFiles[j]);
              strcpy(origNameFiles[j], origNameFiles[j + 1]);
              strcpy(origNameFiles[j + 1], temp);


              temp = (char*)calloc(30, sizeof(char));
              strcpy(temp, nextLevelFiles[j]);
              strcpy(nextLevelFiles[j], nextLevelFiles[j + 1]);
              strcpy(nextLevelFiles[j + 1], temp);
          }

      }

  }


  firstLevelValues(path, i, 0); // the one signifies it'll output tree information

  closedir(dir);
}


void stringFunction(char * path, struct dirent *filePtr) // get number of folders and files
{

  if(strcmp(filePtr->d_name, ".") == 0 || strcmp(filePtr->d_name, "..") == 0)
  {
      return ;
  }

  char tempString[1000][1000];
  strcpy(tempString[0], filePtr->d_name);

  char temp[1000][1000];
  if(strlen(filePtr->d_name) > 0)
  {


    strcpy(temp[1], "");
    strcat(temp[1], path);
    strcat(temp[1], "/");


    strcat(temp[1], filePtr->d_name);
    strcpy(filePtr->d_name , temp[1]);




    struct stat fileStat;


    if(stat(filePtr->d_name, &fileStat) < 0)
        return;    
  }

  struct stat sb;

  if (stat(filePtr->d_name, &sb) == -1) {
      // perror("stat");
  }


  switch (sb.st_mode & S_IFMT) // find out if its a file or folder
  {
    case S_IFDIR:  // files
    if(strcmp(filePtr->d_name, ".") != 0 && strcmp(filePtr->d_name, "..") != 0)
    {
        strcpy(nextLevelFolders[indexValueFolder], filePtr->d_name);
        strcpy(origNameFolders[indexValueFolder], tempString[0]);

        indexValueFolder++;
    }

    break;
    case S_IFREG: // foldaers 
    if(strcmp(filePtr->d_name, ".") != 0 && strcmp(filePtr->d_name, "..") != 0)
    {
        strcpy(nextLevelFiles[indexValueFiles], filePtr->d_name);
        strcpy(origNameFiles[indexValueFiles], tempString[0]);

        indexValueFiles++;
    }

    break;

    default:
    break;
  }

}


void newLevel(char * path, int placement) // get the new level information
{

  int tempCount = 0;

  for(int x = 0; x < indexValueFolder; x++)
  {
      if(strcmp(nextLevelFolders[x], "") != 0)
      {

          tempCount++;

          if(tempCount == 1 && placement == 0)
          {
              printf("\nDirectories\n"); // print out directories label
          }

          strcpy(path, nextLevelFolders[x]);

          getCurrentLevelInfoFolders(path, x, placement);

          
          strcpy(oldLevelFolders[indexValueOldFolder], nextLevelFolders[x]);
          strcpy(origOldNameFolders[indexValueOldFolder], origNameFolders[x]);

          indexValueOldFolder++;

          strcpy(nextLevelFolders[x], "");
          strcpy(origNameFolders[x], "");


      }
  }

  tempCount = 0;

  for(int y = 0; y < indexValueFiles; y++)
  {
      if(strcmp(nextLevelFiles[y], "") != 0 )
      {
          tempCount++;

          if(tempCount == 1 && placement == 0)
          {
              printf("\nFiles\n"); // print out files label
          }

          strcpy(path, nextLevelFiles[y]);


          getCurrentLevelInfoFiles(path, y, placement);
          strcpy(nextLevelFiles[y], "");
          strcpy(origNameFiles[y], "");



      }
  }

  if(placement == 1)
  {

    int inodeCalc = 0;

      for(int i = 0; i < inodeFolderNumber; i++)
      {
        inodeCalc += inodeLevelArray[i];
      }

      inodeLevelArray[inodeFolderNumber] = structCounter - inodeCalc;

      inodeFolderNumber++;


      for (int i = inodeCalc; i < structCounter; i++) // bubble sort inode values
      {
          for (int j = inodeCalc; j < structCounter - 1; j++)
          {

              if (inodeNo[j] > inodeNo[j + 1]) 
              {
                  int temp;
                  char* tempString;

                  temp =  inodeNo[j];
                  inodeNo[j] =  inodeNo[j + 1];
                  inodeNo[j + 1] = temp;


                  temp =  bytes[j];
                  bytes[j] =  bytes[j + 1];
                  bytes[j + 1] = temp;


                  temp =  fileSize[j];
                  fileSize[j] =  fileSize[j + 1];
                  fileSize[j + 1] = temp;


                  temp =  blockSize[j];
                  blockSize[j] =  blockSize[j + 1];
                  blockSize[j + 1] = temp;

                  temp =  levelId[j];
                  levelId[j] =  levelId[j + 1];
                  levelId[j + 1] = temp;


                  tempString = (char*)calloc(30, sizeof(char));
                  strcpy(tempString, dirName[j]);
                  strcpy(dirName[j], dirName[j + 1]);
                  strcpy(dirName[j + 1], tempString);


              }

          }

        }

        for (int i = inodeCalc; i < structCounter; i++) // print out inode values
        {

            printf("%d\t", inodeNo[i]);
            printf("%d\t", bytes[i]);
            printf("%d\t", fileSize[i]);
            printf("%d\t", blockSize[i]);
            printf("%s\n", dirName[i]);

        }

    }


}


void firstLevelValues(char * path, int i, int placement) // get first levels information
{

  numberOfFoldersInLevel[levelNumber] = indexValueFolder;

  int tempCount = 0;

  for(int i = 0; i < indexValueFolder; i++)
  {
      if(strcmp(nextLevelFolders[i], "") != 0)
      {
          tempCount++;
          if(tempCount == 1 && placement == 0)
          {
              printf("\nDirectories\n"); // print out directories label
          }


          getCurrentLevelInfoFolders(path, i, placement); // get current level

          
          strcpy(oldLevelFolders[indexValueOldFolder], nextLevelFolders[i]);
          strcpy(origOldNameFolders[indexValueOldFolder], origNameFolders[i]);


          indexValueOldFolder++;
          strcpy(nextLevelFolders[i], "");
          strcpy(origNameFolders[i], "");

      }

  }


  tempCount = 0;

  for(int j = 0; j < indexValueFiles; j++)
  {
      if(strcmp(nextLevelFiles[j], "") != 0 )
      {
          tempCount++;
          if(tempCount == 1 && placement == 0)
          {
              printf("\nFiles\n"); // print out files label
          }


          getCurrentLevelInfoFiles(path, j, placement);
          strcpy(nextLevelFiles[j], "");
          strcpy(origNameFiles[j], "");

      }

  }

  int count = 0;
  int calc = 0;
  int tempC = 0;


  if(levelCount == 1)
  {
    levelCount++;
  }


  int inodeCheck = 0;

  // indexValueFolder = 1;
  if(placement == 1)
  {
    if(indexValueFolder == 0)
    {
      indexValueFolder = indexValueFiles;
      inodeCheck = 1;
    }

    if(indexValueFiles == 0)
    {
      indexValueFiles = indexValueFolder;
    }
  }

  while(count < indexValueFolder)
  {



    if(placement == 1) // inode report
    {

      int inodeCalc = 0;
      
      if(count == 0)
      {
          inodeLevelArray[inodeFolderNumber] = structCounter;
      }
      else
      {
        for(int i = 0; i < inodeFolderNumber; i++)
        {
          inodeCalc += inodeLevelArray[i];
        }
        
        inodeLevelArray[inodeFolderNumber] = structCounter - inodeCalc;

      }
      
      inodeFolderNumber++;


      for (i = inodeCalc; i < structCounter; i++) // bubble sort inode values
      {
          for (int j = inodeCalc; j < structCounter - 1; j++)
          {

              if (inodeNo[j] > inodeNo[j + 1]) 
              {
                  int temp;
                  char* tempString;

                  temp =  inodeNo[j];
                  inodeNo[j] =  inodeNo[j + 1];
                  inodeNo[j + 1] = temp;


                  temp =  bytes[j];
                  bytes[j] =  bytes[j + 1];
                  bytes[j + 1] = temp;


                  temp =  fileSize[j];
                  fileSize[j] =  fileSize[j + 1];
                  fileSize[j + 1] = temp;


                  temp =  blockSize[j];
                  blockSize[j] =  blockSize[j + 1];
                  blockSize[j + 1] = temp;

                  temp =  levelId[j];
                  levelId[j] =  levelId[j + 1];
                  levelId[j + 1] = temp;


                  tempString = (char*)calloc(30, sizeof(char));
                  strcpy(tempString, dirName[j]);
                  strcpy(dirName[j], dirName[j + 1]);
                  strcpy(dirName[j + 1], tempString);

              }

          }

      }


      for (int i = inodeCalc; i < structCounter; i++) // print out inode values
      {       

          printf("%d\t", inodeNo[i]);
          printf("%d\t", bytes[i]);
          printf("%d\t", fileSize[i]);
          printf("%d\t", blockSize[i]);
          printf("%s\n", dirName[i]);
      }



    }
    if(placement == 1 && inodeCheck == 1)
    {
      return;
    }


    if(placement == 0) // tree report
    {
      printf("\n\nLevel %d: %s", levelCount, origOldNameFolders[count]);
    }
    else if (placement == 1) // inode report
    {
      printf("\n\nLevel %d Inodes: %s\n", levelCount, origOldNameFolders[count]);

    }


    struct dirent *filePtrNew;
    DIR *dirNew = opendir(oldLevelFolders[count]);
    int i = 0;


    if (!dirNew) // no directory existing
    {
        printf("\nNo directory.\n");
        return; 
    }

    while ((filePtrNew = readdir(dirNew)) != NULL)
    {
        stringFunction(oldLevelFolders[count], filePtrNew); // gets number of files/folders
    }

    int check = 0;

    for(int x = 0; x < 1000; x++)
    {
      check += numberOfFoldersInLevel[x];
    }

    calc = indexValueFolder - check;

    levelNumber++;

    for (i = calc; i < indexValueFolder; i++) // bubble sort folders alphabetically
    {
        for (int j = calc; j < indexValueFolder - 1; j++)
        {

            if (strcmp(origNameFolders[j], origNameFolders[j + 1]) > 0) 
            {
                char* temp;
                temp = (char*)calloc(30, sizeof(char));
                strcpy(temp, origNameFolders[j]);
                strcpy(origNameFolders[j], origNameFolders[j + 1]);
                strcpy(origNameFolders[j + 1], temp);

                temp = (char*)calloc(30, sizeof(char));
                strcpy(temp, nextLevelFolders[j]);
                strcpy(nextLevelFolders[j], nextLevelFolders[j + 1]);
                strcpy(nextLevelFolders[j + 1], temp);
            }
        }

    }

    for (i = calc; i < indexValueFiles; i++) // bubble sort files alphabetically 
    {
        for (int j = calc; j < indexValueFiles - 1; j++)
        {

            if (strcmp(origNameFiles[j], origNameFiles[j + 1]) > 0) 
            {
                char* temp;
                temp = (char*)calloc(30, sizeof(char));
                strcpy(temp, origNameFiles[j]);
                strcpy(origNameFiles[j], origNameFiles[j + 1]);
                strcpy(origNameFiles[j + 1], temp);


                temp = (char*)calloc(30, sizeof(char));
                strcpy(temp, nextLevelFiles[j]);
                strcpy(nextLevelFiles[j], nextLevelFiles[j + 1]);
                strcpy(nextLevelFiles[j + 1], temp);
            }
        }

    }


    newLevel(oldLevelFolders[count], placement); // do new level

    count++;
    tempC++;

    if(tempC == numberOfFoldersInLevel[folderIndex])
    {

        levelCount++;
        folderIndex++;
        numberOfFoldersInLevel[folderIndex] = calc;
        tempC = 0;

    }
    

  }
 
}


void getCurrentLevelInfoFiles(char* path, int j, int placement) // get current information for files
{

  char* line = malloc(1024);
  char* directoryValue = getcwd(line, 1024);
  DIR *cdir = opendir(directoryValue);


  struct stat statStruct;
  stat(nextLevelFiles[j], &statStruct);



  if(placement == 0)
  {

    struct passwd *firstStruct;
    firstStruct = getpwuid(statStruct.st_uid);
    printf("%s", firstStruct->pw_name);

    struct group *secondStruct;
    secondStruct = getgrgid(statStruct.st_gid);
    //first value
    printf("(%s)\t", secondStruct->gr_name);

    //second value
    printf("%llu\t", statStruct.st_ino);

    // permissions
    printf( (S_ISDIR(statStruct.st_mode)) ? "d" : "-");
    printf( (statStruct.st_mode & S_IRUSR) ? "r" : "-");
    printf( (statStruct.st_mode & S_IWUSR) ? "w" : "-");
    printf( (statStruct.st_mode & S_IXUSR) ? "x" : "-");
    printf( (statStruct.st_mode & S_IRGRP) ? "r" : "-");
    printf( (statStruct.st_mode & S_IWGRP) ? "w" : "-");
    printf( (statStruct.st_mode & S_IXGRP) ? "x" : "-");
    printf( (statStruct.st_mode & S_IROTH) ? "r" : "-");
    printf( (statStruct.st_mode & S_IWOTH) ? "w" : "-");
    printf( (statStruct.st_mode & S_IXOTH) ? "x" : "-");

    // size
    printf( "\t%llu\t", statStruct.st_size);


    // last modification date
    struct tm *getDate;
    char getSecondTime[20];
    getDate = localtime(&statStruct.st_mtime);
    strftime(getSecondTime, sizeof(getSecondTime), "%b %d %H:%M", getDate);

    // file name
    printf(" %s\n", origNameFiles[j]);
    // first time
    printf("%s", ctime(&statStruct.st_atime));
    // second time
    printf("%s\n", ctime(&statStruct.st_mtime));
  }
  else if (placement == 1)
  {
    // do not print, just store values for inode
    float value1 = statStruct.st_size;
    float divideFileSize = value1 / 512;
    divideFileSize = ceil(divideFileSize);
    int finalValue = (int)divideFileSize;

    inodeNo[structCounter] = statStruct.st_ino;
    bytes[structCounter] = statStruct.st_size;
    fileSize[structCounter] = finalValue;
    blockSize[structCounter] = statStruct.st_blocks;
    strcpy(dirName[structCounter], origNameFiles[j]);
    levelId[structCounter] = levelCount;    
    structCounter++;     

  }

    closedir(cdir);
}

void getCurrentLevelInfoFolders(char* path, int i, int placement)  // get information for folders
{

    char* line = malloc(1024);
    char* directoryValue = getcwd(line, 1024);
    DIR *cdir = opendir(directoryValue);

    struct stat statStruct;


    stat(nextLevelFolders[i], &statStruct);


    if(placement == 0) // tree report
    {
      
      struct passwd *firstStruct;
      firstStruct = getpwuid(statStruct.st_uid);
      printf("%s", firstStruct->pw_name);

      // group name
      struct group *secondStruct;
      secondStruct = getgrgid(statStruct.st_gid);
      printf("(%s)\t ", secondStruct->gr_name);


      printf("%llu\t", statStruct.st_ino);

      // permissions
      printf( (S_ISDIR(statStruct.st_mode)) ? "d" : "-");
      printf( (statStruct.st_mode & S_IRUSR) ? "r" : "-");
      printf( (statStruct.st_mode & S_IWUSR) ? "w" : "-");
      printf( (statStruct.st_mode & S_IXUSR) ? "x" : "-");
      printf( (statStruct.st_mode & S_IRGRP) ? "r" : "-");
      printf( (statStruct.st_mode & S_IWGRP) ? "w" : "-");
      printf( (statStruct.st_mode & S_IXGRP) ? "x" : "-");
      printf( (statStruct.st_mode & S_IROTH) ? "r" : "-");
      printf( (statStruct.st_mode & S_IWOTH) ? "w" : "-");
      printf( (statStruct.st_mode & S_IXOTH) ? "x" : "-");

      // size
      printf("\t%llu\t", statStruct.st_size);

      // last modification date
      struct tm *getTime;
      char getSecondTime[20];
      getTime = localtime(&statStruct.st_mtime);
      strftime(getSecondTime, sizeof(getSecondTime), "%b %d %H:%M", getTime);

      // file name
      printf(" %s\n", origNameFolders[i]);
      
      printf("%s", ctime(&statStruct.st_atime));
        
      printf(" %s\n", ctime(&statStruct.st_mtime));

    }
    else if (placement == 1)
    {
      // store inode values, do not print them out until later
      float value1 = statStruct.st_size;
      float divideFileSize = value1 / 512;
      divideFileSize = ceil(divideFileSize);
      int finalValue = (int)divideFileSize;

      inodeNo[structCounter] = statStruct.st_ino;
      bytes[structCounter] = statStruct.st_size;
      fileSize[structCounter] = finalValue;
      blockSize[structCounter] = statStruct.st_blocks;
      strcpy(dirName[structCounter], origNameFolders[i]);
      levelId[structCounter] = levelCount;

      structCounter++;  

    }

    closedir(cdir);
}


