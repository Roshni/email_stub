#include "include/CkImap.h"
#include "include/CkMessageSet.h"
#include "include/CkEmailBundle.h"
#include "include/CkEmail.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	CkImap imap;

	bool success;

	fstream f;

	//  Anything unlocks the component and begins a fully-functional 30-day trial.
	success = imap.UnlockComponent("Anything for 30-day trial");
	if (success != true) {
	printf("%s\n",imap.lastErrorText());
	return 1;
	}

	//  Connect to Gmail IMAP server.
	success = imap.Connect("imap.gmail.com");
	if (success != true) {
	printf("%s\n",imap.lastErrorText());
	return 1;
	}

	imap.put_Ssl(true);
	imap.put_Port(993);
	//  Login -- eventually read this from file.
	

	string username, password, temp;
//get username and password from xml file
	f.open("user.xml",ios::in);
	while(!f.eof())
	{
		f>>temp;
		int n1, n2;
		if ((n1 = temp.find("<username>")) >=0 ) //if temp contains username, extract it.
		{
			n2 = temp.find("</username>");
			username = temp.substr(n1+10, n2-n1-10);
			cout<<"username extracted "<<username<<endl;
			f>>temp;
			n1 = temp.find("<password>");
			n2 = temp.find("</password>");
			password = temp.substr(n1+10, n2-n1-10);
			cout<<"password extracted "<<password<<endl;
			//cin>>n1;
		}
	}
	cout<<"username: "<<username<<endl;
	cout<<"pswd: "<<password<<endl;
		
	f.close();

//	success = imap.Login("roshnic.tutor@gmail.com","nji98uhb");
	success = imap.Login(username.c_str(),password.c_str());
	if (success != true) {
	printf("%s\n",imap.lastErrorText());
	return 1;
	}

	//  Select an IMAP mailbox
	success = imap.SelectMailbox("Inbox");
	if (success != true) {
	printf("%s\n",imap.lastErrorText());
	return 1;
	}

	CkMessageSet *messageSet = 0;
	bool fetchUids;
	fetchUids = true;

	//  Find emails marked as seen or not already seen:
	const char * seenSearch;
	seenSearch = "SEEN";
	const char * notSeenSearch;
	notSeenSearch = "NOT SEEN";

	//  Get the set of unseen message UIDs
	messageSet = imap.Search(notSeenSearch,fetchUids);
	if (messageSet == 0 ) {
	printf("%s\n",imap.lastErrorText());
	return 1;
	}

	//  Fetch the unseen emails into a bundle object:
	CkEmailBundle *bundle = 0;
	bundle = imap.FetchHeaders(*messageSet);
	if (bundle == 0 ) {
	delete messageSet;
	printf("%s\n",imap.lastErrorText());
	return 1;
	}

	//  Display the Subject and From of each email.
	long i;
	for (i = 0; i <= bundle->get_MessageCount() - 1; i++) {
	CkEmail *email = 0;
	email = bundle->GetEmail(i);

	printf("%s\n",email->getHeaderField("Date"));
	printf("%s\n",email->subject());
	printf("%s\n",email->ck_from());
	printf("--\n");

	delete email;
	}

	//  Disconnect from the IMAP server.
	imap.Disconnect();

	delete messageSet;
	delete bundle;

	return 0;
}
