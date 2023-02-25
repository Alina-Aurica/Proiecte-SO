#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

void listNeRec(const char *path) //listare nerecursiva
{
    DIR *dir = NULL;
    struct dirent *entry;
    char fullPath[512];
    struct stat statbuf;

    if((dir = opendir(path)) == NULL) //se verifica deschiderea directorului
    {
        printf("ERROR\n");
        printf("invalid directory path\n");
        exit(1);
    }

    printf("SUCCESS\n");
    while((entry = readdir(dir)) != NULL) //se citeste din director
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) //se verifica daca nu e folderul curent sau folderul parinte
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            stat(fullPath, &statbuf);
            if(S_ISREG(statbuf.st_mode)) //se verifica daca e fisier obisnuit
                printf("%s\n", fullPath);
            else if(S_ISDIR(statbuf.st_mode)) //se verifica daca e director
                printf("%s\n", fullPath);
        }
    }

    closedir(dir); //se inchide directorul
}

void dimNeRec(const char *path, int value) //listare nerecursiva cu dimensiune pe fisiere prestabilita
{
    DIR *dir;
    struct dirent *entry;
    char fullPath[512];
    struct stat statbuf;

    if((dir = opendir(path)) == NULL) //se verifica deschiderea directorului
    {
        printf("ERROR\n");
        printf("invalid directory path\n");
        exit(1);
    }

    printf("SUCCESS\n");
    while((entry = readdir(dir)) != NULL) //se citeste din director
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) //se verifica daca nu e folderul curent sau folderul parinte
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            stat(fullPath, &statbuf);
            if(S_ISREG(statbuf.st_mode)) //daca e fisier, i se calculeaza lungimea in biti
            {
            	int fd = -1;
            	fd = open(fullPath, O_RDONLY); //se deschide fisierul

            	if(fd == -1)
            	{
            		printf("Nu se poate deschide fisierul\n");
            		exit(1);
            	}
		
		int dim_fis = lseek(fd, 0, SEEK_END);
            	if(value < dim_fis) //daca lungimea este mai mare, afisam calea fisierului
            		printf("%s\n", fullPath);

            	close(fd); //se inchide fisierul
            }
        }
    }

    closedir(dir); //se inchide directorul
}

void permNeRec(const char *path) //listare nerecursiva cu permisiune pe fisier
{
    DIR *dir;
    struct dirent *entry;
    char fullPath[512];
    struct stat statbuf;

    if((dir = opendir(path)) == NULL) //se verifica deschiderea directorului
    {
        printf("ERROR\n");
        printf("invalid directory path\n");
        exit(1);
    }

    printf("SUCCESS\n");

    while((entry = readdir(dir)) != NULL) //se citeste din director
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) //se verifica daca nu e folderul curent sau folderul parinte
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            stat(fullPath, &statbuf);

            if(((statbuf.st_mode & 0777)/64)%2 == 1) //daca prima cifra a permisiunii scrisa in octeti este impara, inseamna ca avem permisiune de executie pe owner
            {
                if(S_ISREG(statbuf.st_mode)) //se verifica daca e fisier obisnuit
                {
                    printf("%s\n", fullPath);
                }
                else if(S_ISDIR(statbuf.st_mode)) //se verifica daca e director
                    printf("%s\n", fullPath);
            }
        }
    }

    closedir(dir); //se inchide directorul
}

int one_success = 0; //se ia o variabila globala one_success care ne ajuta sa printam o singura data string-ul success
void listRec(const char *path) //listare recursiva
{
    DIR *dir;
    struct dirent *entry;
    char fullPath[512];
    struct stat statbuf;

    if((dir = opendir(path)) == NULL) //se verifica deschiderea directorului
    {
        printf("ERROR\n");
        printf ("invalid directory path\n");
        exit(1);
    }

    if(one_success == 0)
        printf("SUCCESS\n");

    while((entry = readdir(dir)) != NULL) //se citeste din director
    {
        one_success = 1;
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) //se verifica daca nu e folderul curent sau folderul parinte
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0)
            {
                if(S_ISREG(statbuf.st_mode)) //se verifica daca este fisier obisnuit
                    printf("%s\n", fullPath);
                else if(S_ISDIR(statbuf.st_mode)) //daca este director se apeleaza recursiv functia
                {
                    printf("%s\n", fullPath);
                    listRec(fullPath);
                }

            }
        }
    }

    closedir(dir); //se inchide directorul
}

void dimRec(const char *path, int value) //listare recursiva cu dimensiune pe fisiere prestabilita
{
    DIR *dir;
    struct dirent *entry;
    char fullPath[512];
    struct stat statbuf;

    if((dir = opendir(path)) == NULL) //se verifica deschiderea directorului
    {
        printf("ERROR\n");
        printf ("invalid directory path\n");
        exit(1);
    }

    if(one_success == 0)
        printf("SUCCESS\n");
        
    while((entry = readdir(dir)) != NULL) //se citeste din director
    {
        one_success = 1;
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) //se verifica daca nu e folderul curent sau folderul parinte
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0)
            {
                if(S_ISREG(statbuf.st_mode)) //se verifica daca e fisier normal si i se calculeaza lungimea in octeti
                {
                    int fd = -1;
                    fd = open(fullPath, O_RDONLY); //se deschide fisierul

                    if(fd == -1)
                    {
                        printf("Nu se poate deschide fisierul\n");
                        exit(1);
                    }
                    
                    int dim_fis = lseek(fd, 0, SEEK_END);
                    if(value < dim_fis) //daca lungimea este mai mare, afisam calea fisierului
                        printf("%s\n", fullPath);

                    close(fd); //se inchide fisierul
                }
                else if(S_ISDIR(statbuf.st_mode)) //se verifica daca e director si se apeleaza recursiv daca da
                {
                    //printf("%s\n", fullPath);
                    dimRec(fullPath, value);
                }

            }
        }
    }

    closedir(dir); //se inchide directorul
}

void permRec(const char *path) //listare recursiva cu permisiune pe fisier
{
    DIR *dir;
    struct dirent *entry;
    char fullPath[512];
    struct stat statbuf;

    if((dir = opendir(path)) == NULL) //se verifica deschiderea directorului
    {
        printf("ERROR\n");
        printf("invalid directory path\n");
        exit(-1);
    }

    if(one_success == 0)
        printf("SUCCESS\n");

    while((entry = readdir(dir)) != NULL) //se citeste din director
    {
        one_success = 1;
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) //se verifica daca nu e folderul curent sau folderul parinte
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0)
            {
            	if(((statbuf.st_mode & 0777)/64)%2 == 1) //daca prima cifra a permisiunii scrisa in octeti este impara, inseamna ca avem permisiune de executie pe owner
                {
                    if(S_ISREG(statbuf.st_mode)) //se verifica daca e fisier obisnuit
                    {
                        printf("%s\n", fullPath);
                    }
                    else if(S_ISDIR(statbuf.st_mode)) //se verifica daca e director si se apeleaza recursiv daca da
                    {
                        printf("%s\n", fullPath);
                        permRec(fullPath);
                    }
                }

            }
        }
    }

    closedir(dir); //se inchide directorul
}

void parsare(const char *path) //parsarea
{
	int fd = -1;
	int size_header = 0;
	int version = 0;
	int no_of_sections = 0;
	char caracter;
	int flag = 0;

	fd = open(path, O_RDONLY); //se deschide fisierul

	if(fd == -1) //se verifica daca fisierul a putut fi deschis
	{
		printf("Fisierul nu poate fi deschis\n");
		flag = -1;
	}

	lseek(fd, 0, SEEK_END); //ne mutam la finalul fisierului, ptc header-ul este la final si noi vrem sa citim din el
	lseek(fd, -1, SEEK_END); //pozitionam cursorul in fisier pentru a citi magic-ul

	if(read(fd, &caracter, 1) == -1) //verificam daca am putut citi magic-ul
	{
		printf("Nu putem citi din fisier\n");
		flag = -2;

	}

	if(caracter != 'Y') //verificam daca fisierul are magic-ul diferit de "Y" si ii afisam o eroare
	{
		printf("ERROR\n");
		flag = -3;
		printf("wrong magic\n");
	}

	lseek(fd, -3, SEEK_END); //pozitionam cursorul pentru a citi header_size-ul

	if(read(fd, &size_header, 2) == -1) //verificam daca am putut citi header_size-ul
	{
		printf("Nu putem citi din fisier\n");
		flag = -2;
	}

	lseek(fd, -size_header, SEEK_END); //ne ducem la inceputul header-ului, cu ajutorul header_size-ului

	if(read(fd, &version, 2) == -1) //verificam daca am putut citi version-ul
	{
		printf("Nu putem citi din fisier\n");
		flag = -2;
	}

	if(!(version >= 45 && version <= 69)) //verificam daca version-ul nu se afla in intervalul dat
	{
		printf("ERROR\n");
		flag = -3;
		printf("wrong version\n");
	}

	lseek(fd, -size_header + 2, SEEK_END); //mutam cursorul pentru a citi no_of_sections

	if(read(fd, &no_of_sections, 1) == -1) //verificam daca am putut citi version-ul
	{
		printf("Nu putem citi din fisier\n");
		flag = -2;
	}

	if(!(no_of_sections >= 2 && no_of_sections <= 15)) //verificam daca no+of_section-ul nu se afla in intervalul dat
	{
		printf("ERROR\n");
		flag = -3;
		printf("wrong sect_nr\n");
	}

	size_header = size_header - 3;

	char name[no_of_sections][16];
	int types[no_of_sections];
	int sizes[no_of_sections];


	lseek(fd, -size_header, SEEK_END); //ne ducem la inceputul header_section-ului


	for(int i = 0; i < no_of_sections; i++) //parcurgem fiecare sectiune in parte
	{
		if(read(fd, name[i], 15) == -1) //citim numele sectiunii
		{
			printf("Nu putem citi din fisier\n");
			flag = -2;
		}
		name[i][15] = '\0'; //adaugam la final '\0'

		if(read(fd, &types[i], 4) == -1) //citim tipul sectiunii
		{
			printf("Nu putem citi din fisier\n");
			flag = -2;
		}

		if(!(types[i] == 60 || types[i] == 73 || types[i] == 83 || types[i] == 79 || types[i] == 28 || types[i] == 87)) //verificam tipul sectiunii
		{
			printf("ERROR\n");
			flag = -3;
			printf("wrong sect_types\n");
			break; //in situatia in care tipul nu e cel dorit, parasim for-ul
		}

		lseek(fd, 4, SEEK_CUR); //sarim peste section offset

		if(read(fd, &sizes[i], 4) == -1) //citim dimensiunea sectiunii
		{
			printf("Nu putem citi din fisier\n");
			flag = -2;
		}

	}

	if(flag == 0) //daca nu a aparut nicio eroare, adica flag-ul a ramas 0, inseamna ca fisierul este SF si ii printam toate caracteristicile
	{
		printf("SUCCESS\n");
		printf("version=%d\n", version);
		printf("nr_sections=%d\n", no_of_sections);
		for(int i = 0; i < no_of_sections; i++)
		{
			printf("section%d: %s %d %d\n", i+1, name[i], types[i], sizes[i]);
		}
	}

	close(fd); //inchidem fisierul


}

void extragere(const char *path, int nr_section, int nr_line) //extragerea unei linii de la o anumita sectiune
{
	int fd = -1;
	int size_header = 0;
	int version = 0;
	int no_of_sections = 0;
	char caracter;
	int flag = 0;
	int poz_cur = 1;
	int j = 0;

	fd = open(path, O_RDONLY); //deschidem fisierul

	if(fd == -1) //verificam daca s-a deschis corespunzator
	{
		printf("ERROR\n");
		printf("invalid file\n");
		flag = -1;
	}

    //verificam mai departe daca fisierul este SF
	lseek(fd, 0, SEEK_END);
	lseek(fd, -1, SEEK_END);

	if(read(fd, &caracter, 1) == -1)
	{
		printf("Nu putem citi din fisier");
		flag = -2;

	}

	if(caracter != 'Y')
		flag = -3;

	lseek(fd, -3, SEEK_END);

	if(read(fd, &size_header, 2) == -1)
	{
		printf("Nu putem citi din fisier\n");
		flag = -2;
	}

	lseek(fd, -size_header, SEEK_END);

	if(read(fd, &version, 2) == -1)
	{
		printf("Nu putem citi din fisier\n");
		flag = -2;
	}

	if(!(version >= 45 && version <= 69))
		flag = -3;

	lseek(fd, -size_header + 2, SEEK_END);

	if(read(fd, &no_of_sections, 1) == -1)
	{
		printf("Nu putem citi din fisier\n");
		flag = -2;
	}

	if(!(no_of_sections >= 2 && no_of_sections <= 15))
		flag = -3;

	size_header = size_header - 3;

	char name[no_of_sections][16];
	int types[no_of_sections];
	int offset[no_of_sections];
	int sizes[no_of_sections];


	lseek(fd, -size_header, SEEK_END);


	for(int i = 0; i < no_of_sections; i++)
	{
		if(read(fd, name[i], 15) == -1)
		{
			printf("Nu putem citi din fisier\n");
			flag = -2;
		}
		name[i][15] = '\0';

		if(read(fd, &types[i], 4) == -1)
		{
			printf("Nu putem citi din fisier\n");
			flag = -2;
		}

		if(!(types[i] == 60 || types[i] == 73 || types[i] == 83 || types[i] == 79 || types[i] == 28 || types[i] == 87))
		{
			flag = -3;
			break;
		}

		if(read(fd, &offset[i], 4) == -1)
		{
			printf("Nu putem citi din fisier\n");
			flag = -2;
		}

		if(read(fd, &sizes[i], 4) == -1)
		{
			printf("Nu putem citi din fisier\n");
			flag = -2;
		}

	}

    //daca fisierul este SF
	if(flag == 0)
	{
		if(no_of_sections < nr_section) //verificam daca exista acea sectiune
		{
			printf("ERROR\n");
			printf("invalid section\n");
		}
		else
		{
			lseek(fd, offset[nr_section-1], SEEK_SET); //citim de la offset-ul sectiunii noastre

			char buff[sizes[nr_section-1]]; //instantium buffere
			char buff2[sizes[nr_section-1]];
			char buff_invers[sizes[nr_section-1]];

			int nr_biti_cititi = read(fd, buff, sizes[nr_section-1]); //citim in buff toata sectiunea

			if(nr_biti_cititi == -1) // verificam daca s-a putut citi
			{
				printf("Nu putem citi din fisier\n");
				flag = -2;
			}

			for(int i = nr_biti_cititi - 1; i >= 0 ; i--) //liniile sunt numerotate de la finalul sectiunii in sus
			{
				if(poz_cur == nr_line) //verificam daca pozitia curenta e cea a liniei pe care o cautam
				{
					buff2[j]=buff[i]; //adaugam in buff2, care e o variabila intermediara
					j++;
				}

				if(buff[i] == '\n') //pozitia se schimba atunci cand gaseste un nou rand
					poz_cur++;
			}

			if(poz_cur < nr_line) //verificam exista linia pe care vrem sa o afisam
			{
				printf("ERROR\n");
				printf("invalid line\n");
			}
			else
			{

				int k = -1;
				for(int i = j - 1; i >= 0; i--) //inversam caracterele din linie
				{
					k++;
					buff_invers[k] = buff2[i];
				}
				k++;
				buff_invers[k] = '\0';

				printf("SUCCESS");
				printf("%s\n", buff_invers); //afisam linia noastra
			}

		}
	}

	close(fd); //inchidem fisierul

}

void findall(const char *path) //listarea recursiva a fisierelor SF
{
	DIR *dir;
	struct dirent *entry;
	char fullPath[512];
	struct stat statbuf;

	if((dir = opendir(path)) == NULL) //se verifica deschiderea directorului
	{
		printf("ERROR\n");
		printf("Invalid directory path\n");
		exit(1);
	}

	if(one_success == 0)
		printf("SUCCESS\n");

	while((entry = readdir(dir)) != NULL) //se citeste din director
	{
		one_success = 1;
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) //se verifica daca e director curent sau director parinte
		{
			snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
			if(lstat(fullPath, &statbuf) == 0)
			{
				if(S_ISREG(statbuf.st_mode)) //daca e fisier obisnuit, verificam sa vedem daca e SF
				{
					int fd = -1;
					int size_header = 0;
					int version = 0;
					int no_of_sections = 0;
					char caracter;
					int flag = 0;

					fd = open(fullPath, O_RDONLY); //deschidem fisierul

					if(fd == -1) //se verifica daca s-a deschis
					{
						printf("ERROR\n");
						printf("invalid file\n");
						flag = -1;
					}

                    //se verifica daca e SF
					lseek(fd, 0, SEEK_END);
					lseek(fd, -1, SEEK_END);

					if(read(fd, &caracter, 1) == -1)
					{
						printf("Nu putem citi din fisier\n");
						flag = -2;

					}

					if(caracter != 'Y')
						flag = -3;

					lseek(fd, -3, SEEK_END);

					if(read(fd, &size_header, 2) == -1)
					{
						printf("Nu putem citi din fisier\n");
						flag = -2;
					}

					lseek(fd, -size_header, SEEK_END);

					if(read(fd, &version, 2) == -1)
					{
						printf("Nu putem citi din fisier\n");
						flag = -2;
					}

					if(!(version >= 45 && version <= 69))
						flag = -3;


					lseek(fd, -size_header + 2, SEEK_END);

					if(read(fd, &no_of_sections, 1) == -1)
					{
						printf("Nu putem citi din fisier\n");
						flag = -2;
					}

					if(!(no_of_sections >= 2 && no_of_sections <= 15))
						flag = -3;

					size_header = size_header - 3;

					char name[no_of_sections][16];
					int types[no_of_sections];
					int offset[no_of_sections];
					int sizes[no_of_sections];


					lseek(fd, -size_header, SEEK_END);


					for(int i = 0; i < no_of_sections; i++)
					{
						if(read(fd, name[i], 15) == -1)
						{
							printf("Nu putem citi din fisier\n");
							flag = -2;
						}
						name[i][15] = '\0';

						if(read(fd, &types[i], 4) == -1)
						{
							printf("Nu putem citi din fisier\n");
							flag = -2;
						}

						if(!(types[i] == 60 || types[i] == 73 || types[i] == 83 || types[i] == 79 || types[i] == 28 || types[i] == 87))
						{
							flag = -3;
							break;
						}

						if(read(fd, &offset[i], 4) == -1)
						{
							printf("Nu putem citi din fisier\n");
							flag = -2;
						}

						if(read(fd, &sizes[i], 4) == -1)
						{
							printf("Nu putem citi din fisier\n");
							flag = -2;
						}

					}


					if(flag == 0) //daca e SF, trebuie ca macar una din sectiuni sa fie de tip 60
					{
						for(int i = 0; i < no_of_sections; i++)
						{
							if(types[i] == 60)
							{
								flag = 4;
							}

						}

						if(flag == 4) //daca da, afisam path-ul
							printf("%s\n", fullPath);

					}

					close(fd); //inchidem fisierul

				}
				else if(S_ISDIR(statbuf.st_mode)) //daca e director, apelam recursiv
					findall(fullPath);

			}
		}
	}


	closedir(dir); //inchidem fisierul
}


int main(int argc, char **argv)
{
    char path[512];
    int value;
    int no_of_section;
    int no_of_line;
    int flaglist = 0;
    int flagrec = 0;
    int flagsize = 0;
    int flagperm = 0;
    int flagparse = 0;
    int flagextract = 0;
    int flagfind = 0;

    if(argc >= 2)
    {
        if(strcmp(argv[1], "variant") == 0) {
            printf("23210\n");
        }

        for(int i = 1; i < argc; i++)
        {
                if(strcmp(argv[i], "list") == 0) 
                    flaglist = 1;
                    
                if(strcmp(argv[i], "recursive") == 0)
                    flagrec = 1;

                if(strstr(argv[i], "path=") != NULL)
                {
                    sscanf(argv[i], "path=%s", path);
                }

                if(strstr(argv[i], "size_greater=") != NULL)
                {
                    sscanf(argv[i], "size_greater=%d", &value);
                    flagsize = 1;
                }

                if(strcmp(argv[i], "has_perm_execute") == 0)
                {
                    flagperm = 1;
                }
         }

		if(flaglist == 1)
		{
		    if(flagrec == 0)
		    {
		        if(flagsize == 1)
		        {
		            //list cu size
		            dimNeRec(path, value);
		        }
		        else
		        {
		            if(flagperm == 1)
		            {
		            	
		                //list cu perm
		                permNeRec(path);
		            }
		            else
		            { 
		            	//list simpul nerecursiv
		            	listNeRec(path);
		            }

		        }
		    }
		    else if(flagrec == 1)
		    {
		        if(flagsize == 1)
		        {
		            //list cu size
		            dimRec(path, value);
		        }
		        else 
		        {
				if(flagperm == 1)
				{
				    //list cu perm
				    permRec(path);
				}
				else
				{
				    //list simplu recursiv
				    listRec(path);
				}
			}
		    }
              }

        for(int i = 1; i < argc; i++)
        {
        	if(strcmp(argv[i], "parse") == 0)
        		flagparse = 1;

        	if(strstr(argv[i], "path=") != NULL)
        		sscanf(argv[i], "path=%s", path);

        }

        if(flagparse == 1) //parsare
        	parsare(path);

        for(int i = 1; i < argc; i++)
        {
        	if(strcmp(argv[i], "extract") == 0)
        		flagextract = 1;

        	if(strstr(argv[i], "path=") != NULL)
        		sscanf(argv[i], "path=%s", path);

        	if(strstr(argv[i], "section=") != NULL)
        		sscanf(argv[i], "section=%d", &no_of_section);

        	if(strstr(argv[i], "line=") != NULL)
        		sscanf(argv[i], "line=%d", &no_of_line);
        }

        if(flagextract == 1) //extragere
        	extragere(path, no_of_section, no_of_line);

        for(int i = 1; i < argc; i++)
        {
        	if(strcmp(argv[i], "findall") == 0)
        		flagfind = 1;

        	if(strstr(argv[i], "path=") != NULL)
        		sscanf(argv[i], "path=%s", path);

        }

        if(flagfind == 1) //findall
        	findall(path);

    }

    return 0;
}
