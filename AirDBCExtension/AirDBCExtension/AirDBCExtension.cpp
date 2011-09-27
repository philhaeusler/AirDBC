#include <stdlib.h>

#include <excpt.h>
#include <gcroot.h>

#include "AirDBCExtension.h"

using namespace std;
using namespace System;
using namespace System::Data;
using namespace System::Data::OleDb;
using namespace System::Runtime::InteropServices;

extern "C"
{
	gcroot<OleDbConnection^> cachedConnection;

	FREObject open(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		FREObject result;				
		 
		OleDbConnection^ connection = cachedConnection;

		if (connection != nullptr) {
			// If the DBConnection is already connected, then return false;

			FRENewObjectFromBool(false, &result);
			return result;
		}

		char *connectionString;
		uint32_t	connectionStringLength;
		FREGetObjectAsUTF8(argv[0], &connectionStringLength, (const uint8_t **) &connectionString);

		__try
		{
			connection = gcnew OleDbConnection(Marshal::PtrToStringAnsi((IntPtr)connectionString));
			connection->Open();

			cachedConnection = connection;
			FRENewObjectFromBool(true, &result);
		}
		__except(EXCEPTION_EXECUTE_HANDLER) 
		{
			cachedConnection = nullptr;
			FRENewObjectFromBool(false, &result);
		}

		return result;
	}

	FREObject close(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		FREObject result;

		OleDbConnection^ connection = cachedConnection;

		if (connection == nullptr) {
			// If the DBConnection not connected, then return false;
			FRENewObjectFromBool(false, &result);
			return result;
		}

		__try
		{
			connection->Close();
			connection = nullptr;

			cachedConnection = connection;
			FRENewObjectFromBool(true, &result);
		}
		__except(EXCEPTION_EXECUTE_HANDLER) 
		{
			cachedConnection = nullptr;
			FRENewObjectFromBool(false, &result);
		}

		return result;
	}

	FREObject queryForValue(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		char * queryString;
		uint32_t	queryStringLength;
		FREGetObjectAsUTF8(argv[0], &queryStringLength, (const uint8_t **)&queryString);

		OleDbConnection^ connection = cachedConnection;
		OleDbCommand^ command = gcnew OleDbCommand(Marshal::PtrToStringAnsi((IntPtr)queryString), connection);
		
		String^ queryResult = (String^) command->ExecuteScalar();

		FREObject returnVal;
		const uint8_t *result = (const uint8_t*)Marshal::StringToHGlobalAnsi(queryResult).ToPointer(); 
		FRENewObjectFromUTF8(sizeof(result) + 1, result, &returnVal);
		
		return returnVal;
	}
	
	FREObject query(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		char * queryString;
		uint32_t	queryStringLength;
		FREGetObjectAsUTF8(argv[0], &queryStringLength, (const uint8_t **)&queryString);

		FREObject returnVal;
		FRENewObject((const uint8_t*) "Array", 0, NULL, &returnVal, NULL);
		uint32_t resultCount = 0;
		
		OleDbConnection^ connection = cachedConnection;
		if (connection == nullptr) {
			FRENewObjectFromBool(false, &returnVal);
			return returnVal;
		}

		OleDbCommand^ command;
		OleDbDataReader^ reader;

		__try {
			command = gcnew OleDbCommand(Marshal::PtrToStringAnsi((IntPtr)queryString), connection);
			reader = command->ExecuteReader(CommandBehavior::KeyInfo);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			FRENewObjectFromBool(false, &returnVal);
			return returnVal;
		}

		while(reader->Read()) {
			FREObject objectValue;
			FRENewObject((const uint8_t*) "Object", 0, NULL, &objectValue, NULL);

			for(int i=0; i<reader->FieldCount; i++) {
				String^ colNameMgd;
				String^ queryResult;

				__try 
				{
					colNameMgd = reader->GetName(i);
					queryResult = reader->GetValue(i)->ToString();
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					continue;
				}

				char *colName = (char*)Marshal::StringToHGlobalAnsi(colNameMgd).ToPointer();
				char *stringValue = (char*)Marshal::StringToHGlobalAnsi(queryResult).ToPointer();

				FREObject value;
				FRENewObjectFromUTF8(sizeof(stringValue) + 1, (const uint8_t *) stringValue, &value);
				FRESetObjectProperty(objectValue, (const uint8_t*) colName, value, NULL);
			}

			resultCount++;
			FRESetArrayLength(returnVal, resultCount);
			FRESetArrayElementAt(returnVal, resultCount-1, objectValue);
		}

		__try {
			reader->Close();
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			;	// just ignore the close exception
		}

		return returnVal;
	}
	
	void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctions, const FRENamedFunction** functions)
	{
		*numFunctions = 4;

		FRENamedFunction* func = (FRENamedFunction*) malloc(sizeof(FRENamedFunction) * (*numFunctions));

		func[0].name = (const uint8_t*) "open";
		func[0].functionData = NULL;
		func[0].function = &open;

		func[1].name = (const uint8_t*) "close";
		func[1].functionData = NULL;
		func[1].function = &close;

		func[2].name = (const uint8_t*) "queryForValue";
		func[2].functionData = NULL;
		func[2].function = &queryForValue;
	
		func[3].name = (const uint8_t*) "query";
		func[3].functionData = NULL;
		func[3].function = &query;

		*functions = func;
	}

	void contextFinalizer(FREContext ctx)
	{
		return;
	}

	void initializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer)
	{
		*ctxInitializer = &contextInitializer;
		*ctxFinalizer = &contextFinalizer;
	}

	void finalizer(void* extData)
	{
		return;
	}
}
