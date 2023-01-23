// NarratorRuntime.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <Engine/StoryNarrator/Includes.h>
#include <iostream>
#include <random>
#include <string>
#include <fstream>

void printUsage()
{
	std::cout << std::endl;
	std::cout << "Usage: " << "StoryNarratorCommandLine.exe" << " [arguments] [options]" << std::endl;
	std::cout << std::endl << "[arguments]" << std::endl;
	std::cout << "(required)<input_story_file>" << "\t" << " - Input File Path" << std::endl;
	std::cout << "(optional)<output_file>" << "\t" << " - Output File Path" << std::endl;
	std::cout << std::endl << "[options]" << std::endl;
	std::cout << "(optional) -p" << "\t" << " - Play Mode" << std::endl;
	std::cout << std::endl;
	std::cout << "------------------------------------------------------------------------" << std::endl;
}

void printInvalidOptions()
{
	std::cout << "Error: Invalid Options." << std::endl;
	printUsage();
}

void printInvalidSyntax()
{
	std::cout << "Error: Invalid Command Syntax." << std::endl;
	printUsage();
}



int main(int argc, char* argv[])
{
	std::string input_file_path;
	std::string output_file_path;
	bool IsPlayMode = false;

	if (argc < 2)
	{
		printUsage();

		input_file_path = "./TestScripts/story_test.txt";
		output_file_path = "./TestScripts/story_test.json";
	}
	else
	{
		if (argc == 2)
		{
			input_file_path = argv[1];
		}
		else if (argc == 3)
		{
			input_file_path = argv[1];
			if (strlen(argv[2]) > 1)
			{
				if (argv[2][0] == '-')
				{
					if (strlen(argv[2]) == 2)
					{
						if (argv[2][1] == 'p')
						{
							//TODO: #NarratorToDo Play Mode
							IsPlayMode = true;
						}
						else
						{
							//TODO: #NarratorToDo Invalid Option
							printInvalidOptions();
							return -1;
						}
					}
					else
					{
						//TODO: #NarratorToDo Invalid Option
						printInvalidOptions();
						return -1;
					}
				}
				else
				{
					output_file_path = argv[2];
				}
			}
			else
			{
				output_file_path = argv[2];
			}
		}
		else if (argc == 4)
		{
			input_file_path = argv[1];

			if (argv[2][0] == '-')
			{
				//TODO: #NarratorToDo Invalid Syntax
				printInvalidSyntax();
				return -1;
			}			

			output_file_path = argv[2];

			if (strlen(argv[3]) > 1)
			{
				if (argv[3][0] == '-')
				{
					if (strlen(argv[3]) > 2)
					{
						if (argv[3][1] == 'p')
						{
							//TODO: #NarratorToDo Play Mode
							IsPlayMode = true;
						}
						else
						{
							//TODO: #NarratorToDo Invalid Option
							printInvalidOptions();
							return -1;
						}
					}
					else
					{
						//TODO: #NarratorToDo Error
						printInvalidOptions();
						return -1;
					}
				}
				else
				{
					//TODO: #NarratorToDo Invalid Syntax
					printInvalidSyntax();
					return -1;
				}
			}
			else
			{
				//TODO: #NarratorToDo Invalid Syntax
				printInvalidSyntax();
				return -1;
			}
		}
	}

	std::ifstream test_input_file(input_file_path);
	if (!test_input_file)
	{
		std::cout << "Error: Input file doesn't exist." << std::endl;
		//printUsage();
		return -1;
	}
	else
	{
		if (output_file_path.empty())
		{
			output_file_path = input_file_path + ".json";
		}

		Narrator::Runtime::Story parsedStory = Narrator::Runtime::Story::Parse(input_file_path, output_file_path, IsPlayMode);

		if (!IsPlayMode)
		{
			if (parsedStory.IsValid())
			{
				Narrator::Runtime::Story story;
				story.FromJSONFile(output_file_path);

				while (story.canRead())
				{
					while (story.canRead())
					{
						std::string dialogue = story.Read();
						if (!dialogue.empty())
						{
							std::cout << dialogue << std::endl;
						}
					}

					std::vector<std::string> choices = story.GetChoices();
					size_t choiceCount = choices.size();
					if (choiceCount > 0)
					{
						size_t choiceIndex = 0;
						for (const std::string& choiceText : choices)
						{
							std::cout << "Choice Index: " << choiceIndex << "\t" << choiceText << std::endl;
							choiceIndex++;
						}

						std::random_device rd;  //Will be used to obtain a seed for the random number engine
						std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
						std::uniform_int_distribution<size_t> distrib(0, (choiceCount - 1));

						auto selectChoiceIndex = distrib(gen);
						std::cout << "Selecting Choice Index: " << selectChoiceIndex << std::endl;
						story.SelectChoice((std::uint32_t)selectChoiceIndex);
					}
					else
					{
						//End of Story
						break;
					}
				}
			}
		}
		else
		{
			if (parsedStory.IsValid())
			{
				Narrator::Runtime::Story story;
				story.FromJSONFile(output_file_path);
				story.Play();
			}
		}

	}

	return 0;
}
