void blackout_conkat(char **args)
{
	int p = 2;
	int i = 1;
	while (args[p] != NULL)
	{
		strcat(args[i], args[p]);
		p++;
	}
	return args[i];
}