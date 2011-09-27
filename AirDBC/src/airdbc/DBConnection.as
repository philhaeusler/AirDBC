package airdbc
{
	import flash.external.ExtensionContext;
	
	public class DBConnection
	{
		private var context : ExtensionContext;
		private var isConnected : Boolean;
		
		public function DBConnection(connection:String=null)
		{
			context = ExtensionContext.createExtensionContext('airdbc.DBConnection', '');
			isConnected = false;
			
			if (connection != null) {
				open(connection);
			}
		}
		
		public static function get isSupported() : Boolean
		{
			return true;
		}
		
		/**
		 * Opens the specified database connection
		 * 
		 * i.e. "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=c:\\Test.mdb"
		 */
		public function open(connection:String) : Boolean 
		{
			if (isConnected) {
				if (!close()) {
					return false;
				}
			}

			isConnected = context.call('openConnection', connection);
			return isConnected;
		}
		
		/**
		 * Executes the query and returns the results as an Array of Objects
		 */
		public function query(query:String) : Array
		{
			if (!isConnected) throw new Error("Not connected");
			
			return context.call('query', query) as Array;
		}
		
		/**
		 * Executes the query and returns the first column of the first row as a String
		 */
		public function queryForValue(query:String) : String 
		{
			if (!isConnected) throw new Error("Not connected");

			return context.call('queryForValue', query) as String;
		}
		
		/**
		 * Close the connection
		 */
		public function close() : Boolean 
		{
			if (!isConnected) return false;
			
			var result : Boolean = context.call('closeConnection');
			isConnected = !result;
			return result;
		}
		
	}
}