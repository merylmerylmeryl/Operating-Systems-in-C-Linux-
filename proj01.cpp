/*----------------------------------------------------------------------------
 Meryl Mabin
 CSE 410, section 001
 proj01 - Find
 ---------------------------------------------------------------------------*/

using namespace std;
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fnmatch.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <queue>

void readInput( queue<string> &options, queue<string> &targets,
                int ArgCounter, char *Arguments[], bool &recursion_flag );

void Find( queue<string> &options, queue<string> &targets,
           queue<string> t_options, queue<string> t_targets,
           char *Arguments[], bool recursion_flag );

int main( int Argc, char *Arg[] )
{
  DIR* direc;
  queue<string> user_options;			// Queues for user input
  queue<string> user_targets;

  queue<string> temp_options;
  queue<string> temp_targets;

  bool recursion_flag = 0;

  // If directory won't open, print error message and quit
  if ( !(direc = opendir(Arg[1])) )
  {
    cout << "Invalid directory name.\n";
  }

  else
  {
    // Read user input - build the option and target queues
    readInput( user_options, user_targets, Argc, Arg, recursion_flag );

    // Keep an extra copy of the options queue and targets queue
    temp_options = user_options;
    temp_targets = user_targets;

    // If the queues are equal in size, Find
    if ( (user_options.size() == user_targets.size()))
    {
      Find( user_options, user_targets, temp_options, temp_targets, Arg,
            recursion_flag );
    } // end if (queue size check)

    else
    // User input isn't okay, so report error and break.
    {
      cout << "\n*** Invalid option-target combination entered. ***\n";
    }
  } // end directory input check
} // end main



/*----------------------------------------------------------------------------
  Name:    readInput
  Input:   options queue, targets queue, arg counter, arg array
  Purpose: Loop through user arguments and build queues to hold them
----------------------------------------------------------------------------*/
void readInput( queue<string> &options, queue<string> &targets,
                int ArgCounter, char *Arguments[], bool &recursion_flag )
{
  string arg_string;
  recursion_flag = 0;

  // Loop through user arguments
  for ( int I=2; I<ArgCounter; I++ )
  {
    arg_string = Arguments[I];

    // Is it some kind of option?
    if ( arg_string[0] == '-' )
    {
      // Is this a -R (recursive search) option?
      if ( arg_string == "-R" )
      {
        recursion_flag = 1;
      }

      // Is this a different option? Then put it on the options stack.
      else if ( arg_string == "-name" )
      {
        options.push( arg_string );
      }
      else if ( arg_string == "-size" )
      {
        options.push( arg_string );
      }
      else if ( arg_string == "-uid" )
      {
        options.push( arg_string );
      }
      else if ( arg_string == "-gid" )
      {
        options.push( arg_string );
      }
      else if ( arg_string == "-atime" )
      {
        options.push( arg_string );
      }
      else if ( arg_string == "-mtime" )
      {
        options.push( arg_string );
      }
      else if ( arg_string == "-ctime" )
      {
        options.push( arg_string );
      }
      else if ( arg_string == "-perm" )
      {
        options.push( arg_string );
      }
    } // end if (option tests)

    else
    // It failed the option test, so it must be a target.
    {
      targets.push( arg_string );
    }
  } // end for (iterating over user args)
}

/*----------------------------------------------------------------------------
  Name:    Find
  Inputs:  option and target queues, temp queues, arguments, flag, directory
  Purpose: Search a directory for files matching the user's search query
----------------------------------------------------------------------------*/
void Find( queue<string> &options, queue<string> &targets,
           queue<string> t_options, queue<string> t_targets,
           char *Arguments[], bool recursion_flag )
{
  DIR* directory = opendir(Arguments[1]);
  dirent* Reader;				// For reading directories
  string file_name;
  string holder;
  const char *file_path;

  string target;
  string option;
  struct stat buffer;

  string flag = "";
  int a_flag = 0;
  int found_flag = 0;

  string hours,
      minutes,
      seconds,
      month,
      day,
      year,
      target_time,
      file_time,

      s_user_perm,
      s_group_perm,
      s_other_perm;

  unsigned int target_int,
               hundreds,
               tens,
               ones;

  int perm,
      file_perm;

  // If the queues are equal in size, continue
  if ( (options.size() == targets.size()))
  {
    // Read directory until we run out of files
    while ( (Reader = readdir(directory)) != NULL )
    {
      file_name = Reader->d_name;
      holder = Arguments[1] + file_name;

      file_path = holder.c_str();
      stat(file_path, &buffer);

      // For directories, change to that directory and search recursively
      if ( (recursion_flag != 0) && !(chdir(file_path))
           && strcmp((file_name).c_str(), ".")
           && strcmp((file_name).c_str(), "..") )
      {
        directory = opendir(file_path);
        Find( options, targets, t_options, t_targets, Arguments,
              recursion_flag );
      }
      else
      // While the target and option queues are not empty, check each option
      // against its corresponding target (for each file).
      {
        while ( (options.size() > 0) && (targets.size() > 0) )
        {
          // Store the top elements of each queue
          option = options.front();
	  target = targets.front();

	  // Is this a -name option search?
	  if ( option == "-name" )
	  {
            if ( !fnmatch(target.c_str(), file_name.c_str(), a_flag) )
	    {
	 //     flag = file_path;
              flag = file_name;
	    }
            else
	    {
              flag = "";
              break;
	    }
            options.pop();
	    targets.pop();
            continue;
          } // end -name option

	  // Is this a -size option search?
	  else if ( option == "-size" )
          {
	    if ( atoi(target.c_str()) == buffer.st_size )
            {
//              flag = file_path;
              flag = file_name;
            }
	    else
	    {
	      flag = "";
              break;
	    }
	    options.pop();
	    targets.pop();
            continue;
	  } // end -size option

	  // Is this a -uid option search?
	  else if ( option == "-uid" )
          {
            target_int = atoi(target.c_str());
	    if ( target_int == buffer.st_uid )
            {
//              flag = file_path;
              flag = file_name;
            }
	    else
	    {
	      flag = "";
              break;
	    }
	    options.pop();
	    targets.pop();
            continue;
	  } // end -uid option

	  // Is this a -gid option search?
	  else if ( option == "-gid" )
          {
            target_int = atoi(target.c_str());
	    if ( target_int == buffer.st_gid )
            {
              flag = file_name;
//              flag = file_path;
            }
	    else
	    {
	      flag = "";
              break;
	    }
	    options.pop();
	    targets.pop();
            continue;
	  } // end -gid option

	  // Is this a -atime option search?
	  else if ( option == "-atime" )
          {
//            cout << ctime(&buffer.st_atime) << "\n";
            if ( !fnmatch("*:*:*-*/*/*", target.c_str(), 0)
                 && strlen(target.c_str()) == 17 )
            {
              // Parse the string for hours, minutes, etc.
              hours = target.substr(0, 2);
              minutes = target.substr(3, 2);
              seconds = target.substr(6, 2);
              month = target.substr(9, 2);
              day = target.substr(12, 2);
              year = target.substr(15, 2);

              // Turn the numerical month into its name abbreviation
              if ( month == "01" )
              {
                month = "Jan";
              }
              else if ( month == "02" )
              {
                month = "Feb";
              }
              else if ( month == "03" )
              {
                month = "Mar";
              }
              else if ( month == "04" )
              {
                month = "Apr";
              }
              else if ( month == "05" )
              {
                month = "May";
              }
              else if ( month == "06" )
              {
                month = "Jun";
              }
              else if ( month == "07" )
              {
                month = "Jul";
              }
              else if ( month == "08" )
              {
                month = "Aug";
              }
              else if ( month == "09" )
              {
                month = "Sep";
              }
              else if ( month == "10" )
              {
                month = "Oct";
              }
              else if ( month == "11" )
              {
                month = "Nov";
              }
              else if ( month == "12" )
              {
                month = "Dec";
              }
              else
              {
                cout << "*** Invalid date entered. ***\n";
                exit(1);
              }

              target_time = month + " " + day + " " + hours + ":" + minutes
                         + ":" + seconds + " 20" + year;

              // Turn the file time into a pattern string
              file_time = ctime(&buffer.st_atime);
              file_time = file_time.substr(4, 20);
              file_time = "*" + file_time;

  //            flag = file_path;
              flag = file_name;
            }
            else
            {
              cout << "*** Invalid date entered. ***\n";
              exit(1);
            }

            if ( !fnmatch( (file_time.c_str()), target_time.c_str(), 0) )
            {
//              flag = file_path;
              flag = file_name;
            }
            else
            {
              flag = "";
            }

	    options.pop();
	    targets.pop();
            continue;
	  } // end -atime option

	  // Is this a -mtime option search?
	  else if ( option == "-mtime" )
          {
            if ( !fnmatch("*:*:*-*/*/*", target.c_str(), 0)
                 && strlen(target.c_str()) == 17 )
            {
              // Parse the string for hours, minutes, etc.
              hours = target.substr(0, 2);
              minutes = target.substr(3, 2);
              seconds = target.substr(6, 2);
              month = target.substr(9, 2);
              day = target.substr(12, 2);
              year = target.substr(15, 2);

              // Turn the numerical month into its name abbreviation
              if ( month == "01" )
              {
                month = "Jan";
              }
              else if ( month == "02" )
              {
                month = "Feb";
              }
              else if ( month == "03" )
              {
                month = "Mar";
              }
              else if ( month == "04" )
              {
                month = "Apr";
              }
              else if ( month == "05" )
              {
                month = "May";
              }
              else if ( month == "06" )
              {
                month = "Jun";
              }
              else if ( month == "07" )
              {
                month = "Jul";
              }
              else if ( month == "08" )
              {
                month = "Aug";
              }
              else if ( month == "09" )
              {
                month = "Sep";
              }
              else if ( month == "10" )
              {
                month = "Oct";
              }
              else if ( month == "11" )
              {
                month = "Nov";
              }
              else if ( month == "12" )
              {
                month = "Dec";
              }
              else
              {
                cout << "*** Invalid date entered. ***\n";
                exit(1);
              }

              target_time = month + " " + day + " " + hours + ":" + minutes
                         + ":" + seconds + " 20" + year;

              // Turn the file time into a pattern string
              file_time = ctime(&buffer.st_mtime);
              file_time = file_time.substr(4, 20);
              file_time = "*" + file_time;

              flag = file_name;
            }
            else
            {
              cout << "*** Invalid date entered. ***\n";
              exit(1);
            }

            if ( !fnmatch( (file_time.c_str()), target_time.c_str(), 0) )
            {
              flag = file_name;
            }
            else
            {
              flag = "";
            }

	    options.pop();
	    targets.pop();
            continue;
	  } // end -mtime option

	  // Is this a -ctime option search?
	  else if ( option == "-ctime" )
          {
            if ( !fnmatch("*:*:*-*/*/*", target.c_str(), 0)
                 && strlen(target.c_str()) == 17 )
            {
              // Parse the string for hours, minutes, etc.
              hours = target.substr(0, 2);
              minutes = target.substr(3, 2);
              seconds = target.substr(6, 2);
              month = target.substr(9, 2);
              day = target.substr(12, 2);
              year = target.substr(15, 2);

              // Turn the numerical month into its name abbreviation
              if ( month == "01" )
              {
                month = "Jan";
              }
              else if ( month == "02" )
              {
                month = "Feb";
              }
              else if ( month == "03" )
              {
                month = "Mar";
              }
              else if ( month == "04" )
              {
                month = "Apr";
              }
              else if ( month == "05" )
              {
                month = "May";
              }
              else if ( month == "06" )
              {
                month = "Jun";
              }
              else if ( month == "07" )
              {
                month = "Jul";
              }
              else if ( month == "08" )
              {
                month = "Aug";
              }
              else if ( month == "09" )
              {
                month = "Sep";
              }
              else if ( month == "10" )
              {
                month = "Oct";
              }
              else if ( month == "11" )
              {
                month = "Nov";
              }
              else if ( month == "12" )
              {
                month = "Dec";
              }
              else
              {
                cout << "*** Invalid date entered. ***\n";
                exit(1);
              }

              target_time = month + " " + day + " " + hours + ":" + minutes
                         + ":" + seconds + " 20" + year;

              // Turn the file time into a pattern string
              file_time = ctime(&buffer.st_ctime);
              file_time = file_time.substr(4, 20);
              file_time = "*" + file_time;

              //flag = file_path;
              flag = file_name;
            }
            else
            {
              cout << "*** Invalid date entered. ***\n";
              exit(1);
            }

            if ( !fnmatch( (file_time.c_str()), target_time.c_str(), 0) )
            {
              flag = file_name;
            }
            else
            {
              flag = "";
            }

	    options.pop();
	    targets.pop();
            continue;
	  } // end -ctime option

          else if ( option == "-perm" )
          {
            // Make sure the permissions option is properly formatted
            if ( (strlen(target.c_str()) != 3)
                 || target[0] < '0' || target[0] > '7'
                 || target[1] < '0' || target[1] > '7'
                 || target[2] < '0' || target[2] > '7')
            {
              cout << "*** Invalid permissions input. ***\n";
              exit(1);
            }
            else
            {
              perm = atoi(target.c_str());
              file_perm = buffer.st_mode;
              hundreds = perm/100;
              tens = perm%100/10;
              ones = perm%10;

              if (    (((buffer.st_mode & S_IRWXU) >> 6) == hundreds)
                   && (((buffer.st_mode & S_IRWXG) >> 3) == tens)
                   && (((buffer.st_mode & S_IRWXO) >> 0) == ones)   )
              {
                flag = file_name;
              }
              else
              {
                flag = "";
              }
            }

          options.pop();
          targets.pop();
          } // end -perm option

        }  // end while (check options/targets for one file; queues now empty)

        // Reset the queues for the next file
        options = t_options;
        targets = t_targets;

        // If the search worked for something, print it out
        if ( !(flag == "") )
        {
          cout << flag << "\n";
          found_flag = 1;
        }
      } // end if (file or directory)
    }// end while (read directory)
    if ( found_flag == 0 )
    {
      cout << "*** No matches found. ***\n";
    }
  } // end if (queue size check)
  else
  {
    cout << "*** Invalid user input. ***\n";
  }
}
