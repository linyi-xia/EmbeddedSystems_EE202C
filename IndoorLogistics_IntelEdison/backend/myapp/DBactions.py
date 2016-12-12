import sqlite3

########
# Name: Linyi Xia
# Rev: 0.1
# 
# There are connection and cursor objects when linking a DB
# The connection object can open, close and commit
# The cursor object performs table functions
# The database currently contains car_locations and routes two tables
#######


def connDB(DBname):
	conn = sqlite3.connect(DBname)
	return conn

def printAll(TBname,conn):
	c = conn.cursor()
	statement = 'SELECT * FROM '+TBname + ' ORDER BY ID'
	for row in c.execute(statement):
        	print row

#check status of a car, check status before assigning a new task
def searchByID(TBname,ID,conn):
	c = conn.cursor()
	statement = 'SELECT * FROM '+TBname + ' WHERE ID='+ str(ID)
	print statement
	c.execute(statement)
	return c.fetchall()

#use this to find available cars
def searchByStatus(TBname, Status, conn):
	c = conn.cursor()
	statement = 'SELECT * FROM '+TBname + ' WHERE status='+ str(Status)	
	print statement
        c.execute(statement)
        return c.fetchall()

def writeRoute(Route,ID,conn):
	c = conn.cursor()
	status = searchByID('car_locations',ID,conn)
	if status == 1:
		return 0
	print "status before writing "+ str(status[0][3])
	statement = "UPDATE routes SET route = '"+str(Route)+"' WHERE ID="+str(ID)
	print statement
        c.execute(statement)
	conn.commit()

if __name__=='__main__':
	conn = connDB('locationsNroutes.db')
	printAll('car_locations', conn)
	print searchByID('car_locations',1001,conn)
	print searchByStatus('car_locations','1',conn)
	writeRoute('LLLL',1001,conn)
	print searchByID('routes',1001,conn)
	conn.close()	
