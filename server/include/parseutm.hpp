/*
 * Parses the end of the UTM of the formate <original>?<"SORT="-or-"FILTER="><type-of-sort-or-filter>&<remaining-"SORT="-or-"FILTER="><type-of-sort-or-filter>
 * Sort or filter can be chosen on their own, together, or not at all
 *
 * These two are also valid
 * <original>?<"SORT="-or-"FILTER="><type-of-sort-or-filter>
 * <original>?
 *
 * '&' is used to join a sort and a filter
 * '&' can not be entered as a search character on its own so '%' has to be used as an escape character (i.e. '%&')
 */

#include <iostream>
#include <string>
#include <cstring>

struct utmdata{
	std::string filterstyle;
	int validdata;
	uint32_t sortstyle;
};

void utmparseerr(char utmch, int index)
{
	std::cerr << std::endl << "Error parsing utm" << std::endl << "Character: " << utmch << std::endl << "Index: " << index << std::endl;
	return;
}

utmdata utmparse(std::string utmstr)
{
	/* Initialize a utmdata struct member to add data to
	*/
	struct utmdata mutmdata = {"", 1, 0};

	/* Error handling for empty string */
	if(utmstr.empty())
	{
		mutmdata.validdata = 0;
		std::cerr << std::endl << "String passed to utmparse() is empty" << std::endl;
		return mutmdata;
	}

	/* Array of strings to compare with UTM string and provide integer */
	const char *sortopts[8] = {"NAME", "DATE", "ROCKET", "PROVIDER", "MISSION", "PAD", "LOCATION", "MAXINDEX"};

	const char *sortstylestr = "SORT=";
	const char *filterstylestr = "FILTER=";

	int utmlength = utmstr.length();

	/* Convert to c string with space for terminating character '\0' */
	char cutmstr[utmlength+1];		
	std::strcpy(cutmstr, utmstr.c_str());


	/* i tracks index in UTM */
	int i = 0;

	for(i = 0; i < utmlength; i++)
	{
		/* check for question mark
		 * if found, break
		 */ 
		if(cutmstr[i] == '?')
		{
			break;
		}
	}

	/* If i is set to last index there is no '?'
	*/
	if(i == utmlength)
	{
		mutmdata.validdata = 0;
		std::cerr << std::endl << "No \'?\' character in string passed to utmparse" << std::endl;
		return mutmdata;
	}

	/* Track whether error has printed for improper UTM so that it does not print more than once
	*/
	int errprinted = 0;


	/* j used to compare parts of UTM to find match to string
	 * such as "SORT" 
	 */
	int j = 0;

	int k = 0;

	/* 
	 * For loop to search for first character of attribute
	 * --either sort or filter
	 * Will modify struct if found
	 * Index now set to '?' character
	 * start at next character
	 */
	for(i = i+1; i < utmlength; i++)
	{
		/* Check for match with first character of "SORT" or "FILTER" */
		if(cutmstr[i] == 'S')
		{
			for(i = i+1, j = 1; i < utmlength && j < 5; i++, j++)
			{
				if(cutmstr[i] != sortstylestr[j])
				{
					break;
				}
			}

			if(i == utmlength)
			{
				std::cerr << std::endl << "Missing sort type after \"SORT=\"" << std::endl;
			}
			else if(j == 5)
			{
				for(j = 0; j < 8; j++)
				{
					if(cutmstr[i] == sortopts[j][0])
					{
						i++;
						if(cutmstr[i] == sortopts[j][1])
						{
							for(k = 2, i = i+1; i < utmlength && k < strlen(sortopts[j]); i++, k++)
							{
								if(cutmstr[i] != sortopts[j][k])
								{
									j = 8;
									break;
								}
							}
							if(k < strlen(sortopts[j]))
							{

								if(!errprinted)
								{
									errprinted = 1;
									utmparseerr(cutmstr[i], i);
								}
							}
							else
							{
								mutmdata.sortstyle = j;
							}
						}
						else
						{
							if(j == 3)
							{
								j = 5;

								for(k = 1; i < utmlength && k < strlen(sortopts[j]); i++, k++)
								{
									if(cutmstr[i] != sortopts[j][k])
									{
										j = 8;
										break;
									}
								}
								if(k < strlen(sortopts[j]))
								{
									if(!errprinted)
									{
										errprinted = 1;
										utmparseerr(cutmstr[i], i);
									}
								}
								else
								{
									mutmdata.sortstyle = j;
								}
							}
							else
							{
								j = 7;

								for(k = 1; i < utmlength && k < strlen(sortopts[j]); i++, k++) 
								{
									if(cutmstr[i] != sortopts[j][k])
									{
										j = 8;
										break;
									}
								}
								if(k < strlen(sortopts[j]))
								{
									if(!errprinted)
									{
										errprinted = 1;
										utmparseerr(cutmstr[i], i);
									}
								}
								else
								{
									mutmdata.sortstyle = j;
								}
							}
						}
					}	
				}
				if(i < utmlength - 1)
				{
					if(cutmstr[i] != '&')
					{
						if(!errprinted)
						{
							errprinted = 1;
							utmparseerr(cutmstr[i], i);
						}
					}
					else
						i++;
				}

				for(; i < utmlength; i++)
				{
					/* Check for match with first character of "SORT" or "FILTER" */

					if(cutmstr[i] == 'F')
					{
						for(i = i+1, j = 1; i < utmlength && j < 7; i++, j++)
						{
							if(cutmstr[i] != filterstylestr[j])
							{
								break;
							}
						}	
						if(j < 7)
						{
							if(!errprinted)
							{
								errprinted = 1;
								utmparseerr(cutmstr[i], i);
							}
						}
						else
						{
							while(i < utmlength && cutmstr[i] != '&')
							{
								if(cutmstr[i] == '%')
								{
									i++;
									if(i < utmlength)
									{
										mutmdata.filterstyle += cutmstr[i];
									}
									else
									{
										std::cerr << std::endl << "Escape character with no following character" << std::endl;
									}
								}
								else
								{
									mutmdata.filterstyle += cutmstr[i];
								}

								i++;
							}
						}
						if(i < utmlength)
						{
							if(!errprinted)
							{
								errprinted = 1;
								utmparseerr(cutmstr[i], i);
							}
							i = utmlength;
						}
					}
					else
					{
						/* Error message will be printed if not printed previously */
						if(!errprinted)
						{
							errprinted = 1;
							utmparseerr(cutmstr[i], i);
						}
					}
				}
			}
			else
			{
				if(!errprinted)
				{
					errprinted = 1;
					utmparseerr(cutmstr[i], i);
				}
			}

		}
		else if(cutmstr[i] == 'F')
		{
			for(i = i+1, j = 1; i < utmlength && j < 7; i++, j++)
			{
				if(cutmstr[i] != filterstylestr[j])
				{
					break;
				}
			}	
			if(j < 7)
			{
				if(!errprinted)
				{
					errprinted = 1;
					utmparseerr(cutmstr[i], i);
				}
			}
			else
			{
				while(i < utmlength && cutmstr[i] != '&')
				{
					if(cutmstr[i] == '%')
					{
						i++;
						if(i < utmlength)
						{
							mutmdata.filterstyle += cutmstr[i];
						}
						else
						{
							std::cerr << std::endl << "Escape character with no following character" << std::endl;
						}
					}
					else
					{
						mutmdata.filterstyle += cutmstr[i];
					}

					i++;
				}
				for(i = i + 1; i < utmlength; i++)
				{
					/* Check for match with first character of "SORT" or "FILTER" */
					if(cutmstr[i] == 'S')
					{
						for(i = i+1, j = 1; i < utmlength && j < 5; i++, j++)
						{
							if(cutmstr[i] != sortstylestr[j])
							{
								break;
							}
						}

						if(i == utmlength)
						{
							std::cerr << std::endl << "Missing sort type after \"SORT=\"" << std::endl;
						}
						else if(j == 5)
						{
							for(j = 0; j < 8; j++)
							{
								if(cutmstr[i] == sortopts[j][0])
								{
									i++;
									if(cutmstr[i] == sortopts[j][1])
									{
										for(k = 2, i = i+1; i < utmlength && k < strlen(sortopts[j]); i++, k++)
										{
											if(cutmstr[i] != sortopts[j][k])
											{
												j = 8;
												break;
											}
										}
										if(k < strlen(sortopts[j]))
										{

											if(!errprinted)
											{
												errprinted = 1;
												utmparseerr(cutmstr[i], i);
											}
										}
										else
										{
											mutmdata.sortstyle = j;
										}
									}
									else
									{
										if(j == 3)
										{
											j = 5;

											for(k = 1; i < utmlength && k < strlen(sortopts[j]); i++, k++)
											{
												if(cutmstr[i] != sortopts[j][k])
												{
													j = 8;
													break;
												}
											}
											if(k < strlen(sortopts[j]))
											{
												if(!errprinted)
												{
													errprinted = 1;
													utmparseerr(cutmstr[i], i);
												}
											}
											else
											{
												mutmdata.sortstyle = j;
											}
										}
										else
										{
											j = 7;

											for(k = 1; i < utmlength && k < strlen(sortopts[j]); i++, k++) 
											{
												if(cutmstr[i] != sortopts[j][k])
												{
													j = 8;
													break;
												}
											}
											if(k < strlen(sortopts[j]))
											{
												if(!errprinted)
												{
													errprinted = 1;
													utmparseerr(cutmstr[i], i);
												}
											}
											else
											{
												mutmdata.sortstyle = j;
											}
										}
									}
								}	
							}

							if(i < utmlength)
							{
								if(!errprinted)
								{
									errprinted = 1;
									utmparseerr(cutmstr[i], i);
								}
								i = utmlength;
							}

						}
						else
						{
							if(!errprinted)
							{
								errprinted = 1;
								utmparseerr(cutmstr[i], i);
							}
						}

					}

					else
					{
						/* Error message will be printed if not printed previously */
						if(!errprinted)
						{
							errprinted = 1;
							utmparseerr(cutmstr[i], i);
						}
					}
				}
			}
		}
		else
		{
			/* Error message will be printed if not printed previously */
			if(!errprinted)
			{
				errprinted = 1;
				utmparseerr(cutmstr[i], i);
			}
		}
	}

	return mutmdata;
}
