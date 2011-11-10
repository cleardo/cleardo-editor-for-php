let php_path = "/usr/local/phpcgi/bin/php ";;
let init_cmd = php_path^"-c ./myphp.ini init_ts.php" in Sys.command init_cmd;;
let in_file = open_in "first_temp.txt";;
let r = ref 0;;
let myqueue = Queue.create ();;
        try
                while !r < 10 do 
                        let x = input_line in_file in  
                        Queue.push x myqueue;
                        r := !r + 1
                done
        with End_of_file -> close_in in_file;;
let endts = Unix.time();;
let endts_int = Int32.of_float endts;;
let daysecs = Int32.of_int (3600*24*10);;
let stop_show = fun res -> if (res = 0) then print_string "";;
let go = fun myarg -> let myts = ref (Int32.of_string myarg) in while
Int32.compare !myts endts_int = -1 do let newarg = Int32.to_string !myts in let
mycmd = php_path^"-c ./myphp.ini stat.php -t"^newarg^" >>php_out.tmp" in let res = Sys.command mycmd in stop_show res;  myts := Int32.add !myts daysecs done;;

let show_procsess a = print_string a;;

let dodo() = print_string "历史数据统计中，请稍后@_@\n";while(true) do Unix.sleep(1);Printf.printf "%c" '.';flush stdout done;;
let pid = Unix.fork();;

if pid = 0 then while(true) do Unix.sleep(1);Printf.printf "%c" '.';flush stdout done
else
        Printf.printf "%s" "历史数据统计中，请稍后@_@\n";flush stdout;
while Queue.length myqueue > 0 do
        let x = Queue.pop myqueue in 
        let t1 = (Thread.create go (x)) in 
        Thread.join t1
done;
Unix.kill pid Sys.sigkill
;;
show_procsess "统计完成\n";;
show_procsess "************************************************\n";;
