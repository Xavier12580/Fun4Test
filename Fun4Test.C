//This is a test program for the Fun4All framework
R__LOAD_LIBRARY(libfun4all)
#include<PHCompositeNode.h>
#include<getClass.h>
#include<PHIODataNode.h>
#include<PHNodeIterator.h>

//We first define a PHObject class to store data in the node tree. This class does nothing but 
//store an integer number 
class Testphobject: public PHObject{public:int a=1;};

//We define two SubsysReco classes here for later use. The first one will multiple our integer number 
//by ten while the other one will add it by one

class Testsubm10: public SubsysReco{
public:
	Testphobject *mydata;
	//InitRun() will be called at the beginning of each run. We use it to find the location of our data
	int InitRun(PHCompositeNode* topnode){
	mydata=findNode::getClass<Testphobject>(topnode,"MYNODE");//our integer number is stored in the node "MYNODE"
	return 0;
	}
	//process_event() will be called for every event. Here we use it to multiple our integer by 10
	int process_event(PHCompositeNode* topnode){
	mydata->a=10*mydata->a;
	return 0;
	}

};

class Testsuba1: public SubsysReco{
public:
	Testphobject *mydata;
	int InitRun(PHCompositeNode* topnode){
	mydata=findNode::getClass<Testphobject>(topnode,"MYNODE");
	return 0;
	}
	int process_event(PHCompositeNode* topnode){
	mydata->a=1+mydata->a;
	return 0;
	}

};

void Fun4Test(){

	Fun4AllServer *se=Fun4AllServer::instance();//We created a Fun4AllServer singleton object
	se->Print();
	//delete se; return; //Let's see what happened when we create our Fun4AllServer. Fisrtly, A Fun4AllHistoManager
	//and a Fun4All Fun4AllSyncManager is created.THhe first object is for histogram management and the second
	//object is for input management. Secondly, a node tree is created, with the top node named "TOP" and three
	//sub nodes named "DST", "RUN" and "PAR" seperately.


	PHCompositeNode *topnode=se->topNode("TOP");//Get the top node
	PHNodeIterator niter(topnode);//Create a node iterator at the top node
	niter.print();//The structure of our node tree is showed
	niter.cd("PAR");//cd to the sub node
	niter.print();//There are nothing in the "RUN" node
	//delete se; return;

	//We now create a new PHCompositeNode and add it to the node "PAR"
	PHCompositeNode *subnode=new PHCompositeNode("SUBNODE");
	niter.addNode(subnode);
	topnode->print();
	//delete se; return 0;//We see a new node "SUBNODE" is added to the node tree

	//This piece shows how to find a specific node in the node tree
	PHNodeIterator topniter(topnode);// Another iterator at top node
	PHCompositeNode *datanode=dynamic_cast<PHCompositeNode *>(topniter.findFirst("PHCompositeNode","SUBNODE"));
	datanode->print();
	//delete;return;//We see we have got the place "SUBNODE"

	//We store our integer number here
	Testphobject *mydata=new Testphobject();
	mydata->a=2;
	PHIODataNode<PHObject> *mynode=new PHIODataNode<PHObject>(mydata,"MYNODE","Testphobject");//A PHIODataNode containing our Testphobject object is created
	datanode->addNode(mynode);
	topniter.print();
	//delete se;return;//Our integer number is added to the node tree!
	
	//This peice shows how to find our integer number in the node tree
	Testphobject *getmydata=findNode::getClass<Testphobject>(topnode,"MYNODE");
	cout<<getmydata->a<<endl;
	//delete se; return;//It's 2


//-----------------Now we use our analysis modules to operate the node tree------------------------

	Testsuba1* a1=new Testsuba1;
	Testsubm10* m10=new Testsubm10;
	se->registerSubsystem(m10);
	se->registerSubsystem(a1);
	se->run(3);
	cout<<getmydata->a<<endl;
	//se->End();delete se; return; //We registered m10 first, and a1 later. We then have 3 events in this run. How much is our number now?
	
	Testsubm10 *m10new=new Testsubm10;
	se->registerSubsystem(m10);
	se->run(1);
	cout<<getmydata->a<<endl;//Now how much is the number?
	
	se->End();
	delete se;
}


