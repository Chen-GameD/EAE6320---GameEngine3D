#pragma once

#include <algorithm> 
#include <cctype>
#include <locale>
#include <functional>
#include <vector>
#include <regex>

namespace Narrator
{
	namespace Runtime
	{
		class StringUtils
		{
		public:
			/*
			 * Trim from start (in place)
			 */
			static inline void LeftTrim(std::string& i_StringToProcess) {
				i_StringToProcess.erase(i_StringToProcess.begin(), std::find_if(i_StringToProcess.begin(), i_StringToProcess.end(), [](unsigned char ch) {
					return !std::isspace(ch);
					}));
			}

			/*
			 * Trim from end (in place)
			 */
			static inline void RightTrim(std::string& i_StringToProcess) {
				i_StringToProcess.erase(std::find_if(i_StringToProcess.rbegin(), i_StringToProcess.rend(), [](unsigned char ch) {
					return !std::isspace(ch);
					}).base(), i_StringToProcess.end());
			}

			/*
			 * Trim from both ends (in place)
			 */
			static inline void Trim(std::string& i_StringToProcess) {
				LeftTrim(i_StringToProcess);
				RightTrim(i_StringToProcess);
			}

			/*
			 * Trim from start (copying)
			 */
			static inline std::string LeftTrimCopy(const std::string& i_StringToProcess) {
				std::string stringCopy = i_StringToProcess;
				LeftTrim(stringCopy);
				return stringCopy;
			}

			/*
			 * Trim from end (copying)
			 */
			static inline std::string RightTrimCopy(const std::string& i_StringToProcess) {
				std::string stringCopy = i_StringToProcess;
				RightTrim(stringCopy);
				return stringCopy;
			}

			/*
			 * Trim from both ends (copying)
			 */
			static inline std::string TrimCopy(const std::string& i_StringToProcess) {
				std::string stringCopy = i_StringToProcess;
				Trim(stringCopy);
				return stringCopy;
			}

			/*
			 * Case Sensitive Implementation
			 */
			static inline bool StartsWith(std::string i_StringToProcess, std::string i_StringToMatch)
			{
				// std::string::find returns 0 if i_StringToMatch is found at starting position
				if (i_StringToProcess.find(i_StringToMatch) == 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			/*
			 * Case Insensitive Implementation
			 */
			static inline bool StartsWithIgnoreCase(std::string i_StringToProcess, std::string i_StringToMatch)
			{
				// Convert i_StringToProcess to lower case
				std::transform(i_StringToProcess.begin(), i_StringToProcess.end(), i_StringToProcess.begin(), ::tolower);
				// Convert i_StringToMatch to lower case
				std::transform(i_StringToMatch.begin(), i_StringToMatch.end(), i_StringToMatch.begin(), ::tolower);
				if (i_StringToProcess.find(i_StringToMatch) == 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			/*
			 * Case Sensitive Implementation - using compare
			 */
			static inline bool EndsWithCompare(const std::string& i_StringToProcess, const std::string& i_StringToMatch)
			{
				if (i_StringToProcess.size() >= i_StringToMatch.size() &&
					i_StringToProcess.compare(i_StringToProcess.size() - i_StringToMatch.size(), i_StringToMatch.size(), i_StringToMatch) == 0)
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			/*
			 * Case Sensitive Implementation
			 */
			static inline bool EndsWith(const std::string& i_StringToProcess, const std::string& i_StringToMatch)
			{
				auto it = i_StringToMatch.begin();
				return i_StringToProcess.size() >= i_StringToMatch.size() &&
					std::all_of(std::next(i_StringToProcess.begin(), i_StringToProcess.size() - i_StringToMatch.size()), i_StringToProcess.end(), [&it](const char& c) {
					return c == *(it++);
						});
			}

			/*
			 * Case Insensitive Implementation
			 */
			static inline bool EndsWithIgnoreCase(std::string i_StringToProcess, std::string i_StringToMatch)
			{
				auto it = i_StringToMatch.begin();
				return i_StringToProcess.size() >= i_StringToMatch.size() &&
					std::all_of(std::next(i_StringToProcess.begin(), i_StringToProcess.size() - i_StringToMatch.size()), i_StringToProcess.end(), [&it](const char& c) {
					return ::tolower(c) == ::tolower(*(it++));
						});
			}

			/*
			 * Erase First Occurrence of given substring
			 */
			static inline void RemoveFirst(std::string& i_StringToProcess, const std::string& i_StringToRemove)
			{
				// Search for the substring in string
				size_t pos = i_StringToProcess.find(i_StringToRemove);
				if (pos != std::string::npos)
				{
					// If found then erase it from string
					i_StringToProcess.erase(pos, i_StringToRemove.length());
				}
			}

			/*
			 * Erase all Occurrences of given substring
			 */
			static inline void RemoveAll(std::string& i_StringToProcess, const std::string& i_StringToRemove)
			{
				size_t pos = std::string::npos;
				// Search for the substring in string in a loop until nothing is found
				while ((pos = i_StringToProcess.find(i_StringToRemove)) != std::string::npos)
				{
					// If found then erase it from string
					i_StringToProcess.erase(pos, i_StringToRemove.length());
				}
			}

			/*
			 * Erase all Occurrences of all given substrings
			 */
			static inline void RemoveAllList(std::string& i_StringToProcess, const std::vector<std::string>& i_ListofStringToRemove)
			{
				// Iterate over the given list of substrings. For each substring call RemoveAll() to
				// remove its all occurrences from main string.
				std::for_each(i_ListofStringToRemove.begin(), i_ListofStringToRemove.end(), std::bind(RemoveAll, std::ref(i_StringToProcess), std::placeholders::_1));
			}

			/*
			* Remove all Whitespace Characters
			*/
			static inline std::string RemoveAllSpaces(const std::string& i_StringToProcess)
			{
				std::string stringCopy = i_StringToProcess;
				/*// defining a regular expression
				std::regex spaceRegEx("\\s+");

				// using the regular expression to remove spaces
				stringCopy = std::regex_replace(stringCopy, spaceRegEx, "");*/

				stringCopy.erase(std::remove_if(stringCopy.begin(), stringCopy.end(), ::isspace),
					stringCopy.end());

				return stringCopy;
			}

			/*
			 * Splits the string into list of substring
			 */
			static inline std::vector<std::string> Split(std::string i_StringToProcess, std::string i_StringDelimiter)
			{
				std::vector<std::string> resultStringList;
				std::size_t startPosition = 0U;
				std::size_t endPosition = i_StringToProcess.find(i_StringDelimiter);
				while (endPosition != std::string::npos)
				{
					std::string splitString = i_StringToProcess.substr(startPosition, endPosition - startPosition);
					resultStringList.emplace_back(splitString);
					startPosition = endPosition + i_StringDelimiter.length();
					endPosition = i_StringToProcess.find(i_StringDelimiter, startPosition);
				}
				std::string splitString = i_StringToProcess.substr(startPosition, endPosition);
				resultStringList.emplace_back(splitString);

				return resultStringList;
			}

			/*
			* Check for Valid Name for Knot
			*/
			static inline bool IsValidKnotName(const std::string& i_StirngToProcess)
			{
				std::string stringToCheck = i_StirngToProcess;
				//RegEx: [a-zA-Z0-9_]+
				std::regex knotPattern("\\w+");
				std::cmatch regexMatch;
				if (std::regex_match(stringToCheck, knotPattern))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		};
	}
}