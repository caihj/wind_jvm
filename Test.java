class Test {
	public static void main(String[] args) {
		System.out.println("Today is a good day！On December 2nd，my jvm can print hello world变长参数~\n");

		for(int i=0;i<args.length;i++)
		    System.out.println(args[i]);
	}
}